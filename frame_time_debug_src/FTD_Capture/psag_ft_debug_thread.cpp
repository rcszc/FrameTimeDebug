// psag_ft_debug_thread.
#include <iostream>
#include <fstream>
#include <filesystem>

#include "psag_ft_debug.hpp"

using namespace std;

namespace FTD_SYS_GLOBAL {
    // windows-specific implementation.
#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#pragma comment(lib, "Psapi.lib")

    id_count_thread GetCurrentThreadID() {
        return (id_count_thread)GetCurrentThreadId();
    }

    size_t GetProcessMemoryUsage() {
        HANDLE HDprocess = GetCurrentProcess();
        if (HDprocess == NULL) return NULL;

        PROCESS_MEMORY_COUNTERS PMC;
        size_t MemoryUsage = 0;
        if (GetProcessMemoryInfo(HDprocess, &PMC, sizeof(PMC)))
            MemoryUsage = PMC.WorkingSetSize;
        return MemoryUsage;
    }

    // linux-specific implementation.
#elif defined(__linux__)
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <unistd.h>

    thread_id_count GetCurrentThreadID() {
        return (thread_id_count)syscall(SYS_gettid);
    }

    size_t GetProcessMemoryUsage() {
        struct sysinfo Info;
        if (sysinfo(&Info) != 0) return NULL;

        long PageSize  = sysconf(_SC_PAGESIZE);
        long PageCount = Info.freeram / PageSize;
        return PageCount * PageSize;
    }
#else
#error "Unsupported Platform!"
#endif
}

namespace FTDthread {
    inline bool DirectoryExists(const string& PATH) {
        // path: current folder.
        if (PATH.empty()) return true;

        filesystem::path _PATH(PATH);
        return filesystem::exists(_PATH) && filesystem::is_directory(_PATH);
    }

    thread*     ProcessThread::GLOBAL_THREAD      = nullptr;
    atomic_bool ProcessThread::GLOBAL_EXITFLAG    = false;
    mutex       ProcessThread::GLOBAL_MUTEX       = {};
    mutex       ProcessThread::GLOBAL_MUTEX_FRAME = {};

    queue<string> ProcessThread::GLOBAL_TEXTS_CACHE = {};
    queue<FTD_SYS_GLOBAL::SrcDataSamplingPoint> ProcessThread::GLOBAL_POINTS_CACHE = {};
    queue<FTD_SYS_GLOBAL::SrcDataSamplingFrame> ProcessThread::GLOBAL_FRAME_CACHE  = {};

#define CONVERT_BIN reinterpret_cast<const char*>
    void ProcessThread::DatasetProcessing(const string& file_path) {
        // files_type: text => .ftbt, points => .ftbp, frame => .ftbf
        fstream DatasetFileText  (file_path + ".ftbt", ios::out | ios::trunc);
        fstream DatasetFilePoints(file_path + ".ftbp", ios::out | ios::trunc | ios::binary);
        fstream DatasetFileFrame (file_path + ".ftbf", ios::out | ios::trunc | ios::binary);

        if (!DatasetFileText.is_open() || !DatasetFilePoints.is_open() || !DatasetFileFrame.is_open()) {
            GLOBAL_EXITFLAG = true;
            cout << "FT_DEBUG: invalid file path, exit." << endl;
        }

        // processing write_loop binary,text.
        while (!GLOBAL_EXITFLAG) {
            {
                unique_lock<mutex> LOCK(GLOBAL_MUTEX);
                while (!GLOBAL_POINTS_CACHE.empty()) {
                    // dataset queue => file(bin).
                    auto BinaryData = SerializeToBinary<FTD_SYS_GLOBAL::SrcDataSamplingPoint>(GLOBAL_POINTS_CACHE.front());
                    // write file(.ftbp), type: binary.
                    DatasetFilePoints.write(CONVERT_BIN(BinaryData.data()), BinaryData.size());
                    GLOBAL_POINTS_CACHE.pop();
                }

                while (!GLOBAL_TEXTS_CACHE.empty()) {
                    // dataset queue => file(text).
                    auto StringData = GLOBAL_TEXTS_CACHE.front();
                    // write file(.ftbt), type: text.
                    DatasetFileText << StringData << endl;
                    GLOBAL_TEXTS_CACHE.pop();
                }
            }
            {
                unique_lock<mutex> LOCK(GLOBAL_MUTEX_FRAME);
                while (!GLOBAL_FRAME_CACHE.empty()) {
                    // dataset queue => file(bin).
                    auto BinaryData = SerializeToBinary<FTD_SYS_GLOBAL::SrcDataSamplingFrame>(GLOBAL_FRAME_CACHE.front());
                    // write file(.ftbf), type: binary.
                    DatasetFileFrame.write(CONVERT_BIN(BinaryData.data()), BinaryData.size());
                    GLOBAL_FRAME_CACHE.pop();
                }
            }
        }
        // close files.
        DatasetFileText .close();
        DatasetFilePoints.close();
        DatasetFileFrame.close();
    }

    bool ProcessThread::CreateProcessingThread(const std::string& folder, const std::string& name) {
        if (!DirectoryExists(folder)) {
            cout << "FT_DEBUG: invalid folder: " << folder << endl;
            return false;
        }
        // file name == null, fmt: in_default name.
        string FileNameStr = name;
        if (name.empty()) 
            FileNameStr = "ftdebug";
        try {
            // craete processing thread(global).
            GLOBAL_THREAD = new thread(DatasetProcessing, folder + FileNameStr);
        }
        catch (const exception& ErrInfo) {
            cout << "FT_DEBUG: failed create thread: " << ErrInfo.what() << endl;
            return false;
        }
        return true;
    }

    bool ProcessThread::DeleteProcessingThread() {
        try {
            GLOBAL_EXITFLAG = true;
            // free thread. join => delete_ptr.
            GLOBAL_THREAD->join(); delete GLOBAL_THREAD;
        }
        catch (const exception& ErrInfo) {
            cout << "FT_DEBUG: failed delete thread: " << ErrInfo.what() << endl;
            return false;
        }
        return true;
    }

    void ProcessThread::MainUpdateFrame() {
        FTD_SYS_GLOBAL::SrcDataSamplingFrame DataTemp = {};
        DataTemp.SmpFrameProcessMemoryUsage = FTD_SYS_GLOBAL::GetProcessMemoryUsage();

        DataTemp.SmpFrameClock  = SamplerFrameRunTimer();
        DataTemp.SmpFrameUnqiue = SamplerFrameUpdate();

        // update frame => get points pos_info.
        PointsCountFrameUpdate(&DataTemp.SmpPointDatasetOffset, &DataTemp.SmpPointDatasetSize);
        DatasetFrameADD(DataTemp);
    }

    void ProcessThread::DatasetPointADD(const FTD_SYS_GLOBAL::SrcDataSamplingPoint& data, const string& text) {
        unique_lock<mutex> LOCK(GLOBAL_MUTEX);
        // points.sampler data => dadaset queue.
        GLOBAL_POINTS_CACHE.push(data);

        if (text.empty()) return;
        GLOBAL_TEXTS_CACHE.push(text);
    }

    void ProcessThread::DatasetFrameADD(const FTD_SYS_GLOBAL::SrcDataSamplingFrame& data) {
        unique_lock<mutex> LOCK(GLOBAL_MUTEX_FRAME);
        // frame.sampler data => dadaset queue.
        GLOBAL_FRAME_CACHE.push(data);
    }

    // DEF: system global_object.
    ProcessThread FTDprocessingThread = {};
}