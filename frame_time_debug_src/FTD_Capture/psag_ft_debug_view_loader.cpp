// psag_ft_debug_view_loader.
#include "psag_ft_debug_view.h"

using namespace std;

#define STRING_NEWLINE_SYMBOL '\n'
#define CONVERT_BIN reinterpret_cast<char*>
namespace FTDViewLoader {

    template <typename T>
    static T DeserializeFromBinary(const vector<uint8_t>& binary_data, bool* error) {
        static_assert(std::is_trivially_copyable<T>::value, "ERROR TYPE!");
        // check deserialize size.
        if (binary_data.size() != sizeof(T)) {
            *error = true;
            return T();
        }
        T DataTemp;
        // binary_data =copy=> struct.
        memcpy(&DataTemp, binary_data.data(), sizeof(T));
        return DataTemp;
    }

    vector<uint8_t> DataFilesLoader::BinaryRead(fstream& FILE_OBJECT) {
        // get dataset size.
        FILE_OBJECT.seekg(0, ios::end);
        size_t FrameDataSize = (size_t)FILE_OBJECT.tellg();
        FILE_OBJECT.seekg(0, ios::beg);

        vector<uint8_t> FrameDatasetTemp(FrameDataSize);
        // read binary dataset.
        FILE_OBJECT.read(CONVERT_BIN(FrameDatasetTemp.data()), FrameDataSize);
        return FrameDatasetTemp;
    }

    vector<string> DataFilesLoader::StringRead(fstream& FILE_OBJECT, size_t* bytes) {
        vector<string> ResultStrArray = {};
        string StringLineTemp = {};

        ResultStrArray.push_back(""); // push null_text.
        // read text_lines dataset.
        while (getline(FILE_OBJECT, StringLineTemp)) {
            // counting bytes.
            *bytes += StringLineTemp.size();
            ResultStrArray.push_back(StringLineTemp);
        }
        return ResultStrArray;
    }

	DataFilesLoader::DataFilesLoader(const string& folder, const string& name) {
        // files_type: text => .ftbt, points => .ftbp, frame => .ftbf
        fstream DatasetFileText  (folder + name + ".ftbt", ios::in);
        fstream DatasetFilePoints(folder + name + ".ftbp", ios::in | ios::binary);
        fstream DatasetFileFrames(folder + name + ".ftbf", ios::in | ios::binary);

        if (!DatasetFileText.is_open() || !DatasetFilePoints.is_open() || !DatasetFileFrames.is_open()) {
            // error message out.
            LoaderErrorInfo += "ftd_err: invalid: path | name."       + STRING_NEWLINE_SYMBOL;
            LoaderErrorInfo += "ftd_err: types: [ftbt] [ftbp] [ftbf]" + STRING_NEWLINE_SYMBOL;
            return;
        }
        // read binary_file(frames) dataset.
        vector<uint8_t> FramesDatasetTemp = BinaryRead(DatasetFileFrames);

        // check dataset incomplete.
        size_t ElementSize = sizeof(FTDViewDataset::SrcDataSamplingFrame);
        if (FramesDatasetTemp.size() % ElementSize != 0) {
            // error message out.
            LoaderErrorInfo = "ftd_err: frames dataset incomplete.";
            return;
        }

        bool ConvertError = false;
        // deserialize frame dataset.
        for (size_t i = 0; i < FramesDatasetTemp.size() / ElementSize; ++i) {
            vector<uint8_t> ElementDataTemp(FramesDatasetTemp.begin() + i * ElementSize, FramesDatasetTemp.begin() + (i + 1) * ElementSize);
            FTDFramesDataset.push_back(
                DeserializeFromBinary<FTDViewDataset::SrcDataSamplingFrame>(ElementDataTemp, &ConvertError)
            );
            if (ConvertError) {
                // error message out.
                LoaderErrorInfo = "ftd_err: frames data deserialize err.";
                return;
            }
        }
        FramesDatasetTemp.clear();
        FramesDatasetTemp.shrink_to_fit();
        cout << FTDFramesDataset.size() << endl;

        // read binary_file(points) dataset.
        vector<uint8_t> PointsDatasetTemp = BinaryRead(DatasetFilePoints);

        // check dataset incomplete.
        ElementSize = sizeof(FTDViewDataset::SrcDataSamplingPoint);
        if (PointsDatasetTemp.size() % ElementSize != 0) {
            // error message out.
            LoaderErrorInfo = "ftd_err: points dataset incomplete.";
            return;
        }

        // 统计 points 中 texts 最大索引.
        FTDViewDataset::id_count_tagging CountTmp = NULL;

        // deserialize points dataset.
        for (size_t i = 0; i < PointsDatasetTemp.size() / ElementSize; ++i) {
            vector<uint8_t> ElementDataTemp(PointsDatasetTemp.begin() + i * ElementSize, PointsDatasetTemp.begin() + (i + 1) * ElementSize);
            auto ReadElement = DeserializeFromBinary<FTDViewDataset::SrcDataSamplingPoint>(ElementDataTemp, &ConvertError);

            // tag count index_max.
            if (ReadElement.SmpPointTagText > CountTmp)
                CountTmp = ReadElement.SmpPointTagText;

            FTDPointsDataset.push_back(ReadElement);
            if (ConvertError) {
                // error message out.
                LoaderErrorInfo = "ftd_err: points data deserialize err.";
                return;
            }
        }
        PointsDatasetTemp.clear();
        PointsDatasetTemp.shrink_to_fit();

        FTDTextsDataset = StringRead(DatasetFileText, &StringArrayBytes);
        if (FTDTextsDataset.size() < CountTmp)
            LoaderErrorInfo = "ftd_err: texts data_size err.";
	}

    size_t DataFilesLoader::GetDatasetElements(size_t* texts, size_t* points, size_t* frame) {
        *texts  = FTDTextsDataset.size();
        *points = FTDPointsDataset.size();
        *frame  = FTDFramesDataset.size();

        // clac total dataset bytes_size.
        return StringArrayBytes * 
            *points * sizeof(FTDViewDataset::SrcDataSamplingPoint) * *frame * sizeof(FTDViewDataset::SrcDataSamplingFrame);
    }
}