// psag_ft_debug_view. RCSZ 2024_08_16. 
// PSAG_FRAME_TIME_DEBUG - x64. (VIEW_EDITOR)

#ifndef __PSAG_FT_DEBUG_VIEW_H
#define __PSAG_FT_DEBUG_VIEW_H
#include <iostream>
#include <fstream>
#include <string>
#include "rcsz_imgui_base/imrcsz_control_base.h"

// MIN Lv.1 => Lv.5 MAJ.  
enum FTD_LEVEL_TAG {
	FTD_TagLv1 = 1 << 1,
	FTD_TagLv2 = 1 << 2,
	FTD_TagLv3 = 1 << 3,
	FTD_TagLv4 = 1 << 4,
	FTD_TagLv5 = 1 << 5
};

namespace FTDViewDataset {
	using pointer_count    = unsigned long long;
	using id_count_thread  = unsigned long long;
	using id_count_tagging = unsigned long;

	// version 0.1, x64, mem: 48bytes, 8bytes. 20240815, update: 20240816.
	struct SrcDataSamplingPoint {
		// timer: milliseconds.
		double SmpPointTimer = 0.0;

		FTD_LEVEL_TAG    SmpPointTagFilter  = FTD_TagLv1;
		pointer_count    SmpPointTagPointer = NULL;
		id_count_tagging SmpPointTagText    = NULL;
		id_count_thread  SmpPointTagThread  = NULL;

		// oper: [-][+] mem_bytes.
		int64_t SmpPointMemOperBytes = NULL;
	};

	// version 0.1, x64, mem: 40bytes, 8bytes. 20240815, update: 20240816.
	struct SrcDataSamplingFrame {
		// index_tabe: addr_offset + size.
		size_t SmpPointDatasetOffset = NULL;
		size_t SmpPointDatasetSize   = NULL;

		size_t SmpFrameUnqiue = NULL;
		double SmpFrameClock  = 0.0;
		size_t SmpFrameProcessMemoryUsage = NULL;
	};

	struct FrameProcessMark {
		size_t SmpPointsLvCount[5] = {};
		// points oper_bytes sum.
		int64_t TotalMemOperBytes = NULL;
	};
}

namespace FTDViewLoader {
	using FTD_TEXTS  = std::vector<std::string>;
	using FTD_POINTS = std::vector<FTDViewDataset::SrcDataSamplingPoint>;
	using FTD_FRAMES = std::vector<FTDViewDataset::SrcDataSamplingFrame>;

	// dataset loader(binary,text): .ftbf .ftbt .ftbp
	class DataFilesLoader {
	protected:
		FTD_TEXTS  FTDTextsDataset  = {};
		FTD_POINTS FTDPointsDataset = {};
		FTD_FRAMES FTDFramesDataset = {};

		std::vector<uint8_t>     BinaryRead(std::fstream& FILE_OBJECT);
		std::vector<std::string> StringRead(std::fstream& FILE_OBJECT, size_t* bytes);
		
		size_t StringArrayBytes = NULL;
	public:
		DataFilesLoader(const std::string& folder, const std::string& name);
		// return total_dataset bytes size.
		size_t GetDatasetElements(size_t* texts, size_t* points, size_t* frame);

		// success: info => empty.
		std::string LoaderErrorInfo = {};

		// get dataset pointer.
		FTD_TEXTS*  GetDatasetTextsIDX()  { return &FTDTextsDataset;  }
		FTD_POINTS* GetDatasetPointsIDX() { return &FTDPointsDataset; }
		FTD_FRAMES* GetDatasetFramesIDX() { return &FTDFramesDataset; }
	};
}

namespace FTDViewEditor {
	// change_rate > v * n.
	constexpr size_t VIEW_ELEMENT_PAGES = 500;

	using FTD_FRAME_MARK = std::vector<FTDViewDataset::FrameProcessMark>;
	// editor draw imgui_panel.
	class DebugViewEditor {
	protected:
		FTDViewLoader::FTD_TEXTS*  TextsDatasetIndex  = nullptr;
		FTDViewLoader::FTD_POINTS* PointsDatasetIndex = nullptr;
		FTDViewLoader::FTD_FRAMES* FramesDatasetIndex = nullptr;

		FTD_FRAME_MARK FrameProcessingMarker = {};

		// scroll.x_pos, scroll.y_lerp.
		ImVec2 FrameViewScroll = {};
		int    FrameViewInTemp = NULL;

		// frames,points page location.
		int FramesViewPageLoc = NULL;
		int PointsViewPageLoc = NULL;

		size_t CurrentViewFrame = NULL;

		size_t FrameSamplePointsBegin = NULL;
		size_t FrameSamplePointsEnd   = NULL;

		std::vector<float> FramesMemoryUsage  = {};
		std::vector<float> FramesIntervalTime = {};

		// limit_value f: x:min, y:max.
		ImVec2 LimitMemoryUsage = {}, LimitTimeInterval = {};

		float ViewPlotValueScale = 0.5f;
		bool  ViewPlotFlagSwitch = false;

		void DrawLocationPlotLines(
			const char* name, const char* tag, 
			const std::vector<float>* sample, size_t count, size_t offset, const ImVec2& value_limit,
			float position
		);
		float ScrollxPosition = 0.0f;

		void DrawColorsLevelMap(const char* name, const FTDViewLoader::FTD_POINTS* sample, size_t count, size_t offset);
		void DrawTotalMarkViews(const char* name, const FTD_FRAME_MARK* sample, size_t count, size_t offset, float position);

		ImVec4 LVTAG_COLOR_FIND(const FTD_LEVEL_TAG& level_tag);
	public:
		DebugViewEditor(FTDViewLoader::DataFilesLoader* object);
		
		float EditorLerpSpeed = 1.0f;
		bool  EditorWinFixed  = true;
		void RenderingViewEditor(const char* w_name);
	};
}

#endif