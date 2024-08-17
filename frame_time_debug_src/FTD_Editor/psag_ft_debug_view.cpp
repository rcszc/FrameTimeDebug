// psag_ft_debug_view.
#include "psag_ft_debug_view.h"

using namespace std;

namespace FTDViewEditor {
	constexpr ImVec4 TagColorLv1 = ImVec4(0.0f, 1.0f, 0.5f, 0.78f);
	constexpr ImVec4 TagColorLv2 = ImVec4(0.0f, 1.0f, 1.0f, 0.78f);
	constexpr ImVec4 TagColorLv3 = ImVec4(1.0f, 1.0f, 0.0f, 0.78f);
	constexpr ImVec4 TagColorLv4 = ImVec4(1.0f, 0.4f, 0.0f, 0.78f);
	constexpr ImVec4 TagColorLv5 = ImVec4(1.0f, 0.0f, 0.0f, 0.78f);

	constexpr FTD_LEVEL_TAG LvList[5] = { FTD_TagLv1,FTD_TagLv2,FTD_TagLv3,FTD_TagLv4,FTD_TagLv5 };

	// draw plot_lines + pos, offset + size.
	void DebugViewEditor::DrawLocationPlotLines(
		const char* name, const char* tag, 
		const vector<float>* sample, size_t count, size_t offset, const ImVec2& value_limit, 
		float position
	) {
		float WindowWidth = ImGui::GetWindowWidth() - IMGUI_ITEM_SPAC * 2.0f;

		ImGui::PushStyleColor(ImGuiCol_ChildBg,   ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
		ImGui::PushStyleColor(ImGuiCol_FrameBg,   ImVec4(0.12f, 0.0f, 0.2f, 0.92f));
		ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4(0.0f, 1.0f, 0.7f, 1.0f));

		ImGui::BeginChild(name, ImVec2(WindowWidth, 50.0f + IMGUI_ITEM_SPAC * 2.0f));
		{
			ImGui::SetWindowFontScale(0.8f);
			ImGui::SetCursorPosY(IMGUI_ITEM_SPAC);
			ImGui::PlotLines(
				name, sample->data() + offset, (int)count, NULL, (const char*)NULL, value_limit.x, value_limit.y,
				ImVec2(WindowWidth - 108.0f, 50.0f)
			);
			// draw line_position.
			RzBase::ListDrawRectangleFill(
				ImVec2((WindowWidth - 108.0f) * position - 2.0f, IMGUI_ITEM_SPAC * 0.5f),
				ImVec2(4.0f, 48.0f + IMGUI_ITEM_SPAC * 1.5f),
				ImVec4(0.72f, 0.72f, 0.72f, 0.58f)
			);
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "%s\nMin: %.2f\nMax: %.2f", tag, value_limit.x, value_limit.y);
		}
		ImGui::EndChild();
		ImGui::PopStyleColor(3);
	}

	void DebugViewEditor::DrawColorsLevelMap(const char* name, const FTDViewLoader::FTD_POINTS* sample, size_t count, size_t offset) {
		float WindowWidth = ImGui::GetWindowWidth() - IMGUI_ITEM_SPAC * 2.0f;
		float DrawStepWidth = (WindowWidth - IMGUI_ITEM_SPAC * 2.0f) / (float)count;

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 1.2f);
		ImGui::BeginChild(name, ImVec2(WindowWidth, 16.0f + IMGUI_ITEM_SPAC));
		{
			for (size_t i = 0; i < count; ++i) {
				RzBase::ListDrawRectangleFill(
					ImVec2((float)i * DrawStepWidth + IMGUI_ITEM_SPAC, IMGUI_ITEM_SPAC), ImVec2(DrawStepWidth, 16.0f - IMGUI_ITEM_SPAC),
					LVTAG_COLOR_FIND((*sample)[offset + i].SmpPointTagFilter)
				);
			}
		}
		ImGui::EndChild();
		ImGui::PopStyleVar(1);
	}

	void DebugViewEditor::DrawTotalMarkViews(
		const char* name, const FTD_FRAME_MARK* sample, size_t count, size_t offset, float position
	) {
		float WindowWidth = ImGui::GetWindowWidth() - IMGUI_ITEM_SPAC * 2.0f;
		float DrawStepWidth = (WindowWidth - IMGUI_ITEM_SPAC * 2.0f) / (float)count;

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 1.2f);
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.0f, 0.2f, 0.92f));
		ImGui::BeginChild(name, ImVec2(WindowWidth, 72.0f + IMGUI_ITEM_SPAC * 2.0f));
		{
			for (size_t i = 0; i < count; ++i) {
				for (size_t j = 2; j < 5; ++j) {

					// marker: filter_tag Lv3-Lv5.
					if ((*sample)[offset + i].SmpPointsLvCount[j])
						RzBase::ListDrawRectangleFill(
							ImVec2((float)i * DrawStepWidth + IMGUI_ITEM_SPAC, 16.0f * float(j - 2) + IMGUI_ITEM_SPAC),
							ImVec2(DrawStepWidth, 14.0f),
							LVTAG_COLOR_FIND(LvList[j])
						);
				}
				// marker: process memory oper.
				if (abs((*sample)[offset + i].TotalMemOperBytes) > 0)
					RzBase::ListDrawRectangleFill(
						ImVec2((float)i * DrawStepWidth + IMGUI_ITEM_SPAC, 48.0f + IMGUI_ITEM_SPAC),
						ImVec2(DrawStepWidth, 24.0f),
						ImVec4(0.78f, 0.1f, 1.0f, 1.0f)
					);
			}
			// draw line_position.
			RzBase::ListDrawRectangleFill(
				ImVec2(WindowWidth * position - 2.0f, IMGUI_ITEM_SPAC * 0.5f),
				ImVec2(4.0f, 72.0f + IMGUI_ITEM_SPAC),
				ImVec4(0.72f, 0.72f, 0.72f, 0.58f)
			);
		}
		ImGui::EndChild();
		ImGui::PopStyleColor(1);
		ImGui::PopStyleVar(1);
	}

	ImVec4 DebugViewEditor::LVTAG_COLOR_FIND(const FTD_LEVEL_TAG& level_tag) {
		switch (level_tag) {
		case(FTD_TagLv1): { return TagColorLv1; }
		case(FTD_TagLv2): { return TagColorLv2; }
		case(FTD_TagLv3): { return TagColorLv3; }
		case(FTD_TagLv4): { return TagColorLv4; }
		case(FTD_TagLv5): { return TagColorLv5; }
		}
		return ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	DebugViewEditor::DebugViewEditor(FTDViewLoader::DataFilesLoader* object) {
		// load dataset pointer.
		TextsDatasetIndex  = object->GetDatasetTextsIDX();
		PointsDatasetIndex = object->GetDatasetPointsIDX();
		FramesDatasetIndex = object->GetDatasetFramesIDX();

		LimitMemoryUsage.x  = FLT_MAX; LimitMemoryUsage.y  = -FLT_MAX;
		LimitTimeInterval.x = FLT_MAX; LimitTimeInterval.y = -FLT_MAX;

		// 第一帧时间无效, time0 - tmp = time1 - time0.
		float FrameTimeTemp = (float)(*FramesDatasetIndex)[1].SmpFrameClock - (float)(*FramesDatasetIndex)[0].SmpFrameClock;
		FrameTimeTemp = (float)(*FramesDatasetIndex)[0].SmpFrameClock - FrameTimeTemp;

		for (const auto& FrameData : *FramesDatasetIndex) {
			float Temp1 = (float)FrameData.SmpFrameProcessMemoryUsage / 1024.0f / 1024.0f;
			float Temp2 = (float)FrameData.SmpFrameClock - FrameTimeTemp;

			// load_array: usage_mem, frame_time.
			FramesMemoryUsage.push_back(Temp1);
			FramesIntervalTime.push_back(Temp2);

			// limit value_calc.
			LimitMemoryUsage.x  = min(LimitMemoryUsage.x, Temp1);  LimitMemoryUsage.y  = max(LimitMemoryUsage.y, Temp1);
			LimitTimeInterval.x = min(LimitTimeInterval.x, Temp2); LimitTimeInterval.y = max(LimitTimeInterval.y, Temp2);
			
			FrameTimeTemp = (float)FrameData.SmpFrameClock;

			FTDViewDataset::FrameProcessMark MarkTemp = {};
			// processing points.
			for (size_t i = FrameData.SmpPointDatasetOffset / sizeof(FTDViewDataset::SrcDataSamplingPoint);
				i < (FrameData.SmpPointDatasetOffset + FrameData.SmpPointDatasetSize) / sizeof(FTDViewDataset::SrcDataSamplingPoint); ++i
			) {
				MarkTemp.TotalMemOperBytes += (int64_t)(*PointsDatasetIndex)[i].SmpPointMemOperBytes;
				// count level(perf).
				switch ((*PointsDatasetIndex)[i].SmpPointTagFilter) {
				case(FTD_TagLv1): { ++MarkTemp.SmpPointsLvCount[0]; break; }
				case(FTD_TagLv2): { ++MarkTemp.SmpPointsLvCount[1]; break; }
				case(FTD_TagLv3): { ++MarkTemp.SmpPointsLvCount[2]; break; }
				case(FTD_TagLv4): { ++MarkTemp.SmpPointsLvCount[3]; break; }
				case(FTD_TagLv5): { ++MarkTemp.SmpPointsLvCount[4]; break; } }
			}
			FrameProcessingMarker.push_back(MarkTemp);
		}
	}

	void DebugViewEditor::RenderingViewEditor(const char* w_name) {
		// [20240816] style_var: 4 items.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 7.2f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,  5.4f);
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding,  7.2f);

		// [20240816] style_color: 8 items.
		ImGui::PushStyleColor(ImGuiCol_WindowBg,  ImVec4(0.09f, 0.09f, 0.09f, 0.92f));
		ImGui::PushStyleColor(ImGuiCol_ChildBg,   ImVec4(0.14f, 0.14f, 0.14f, 0.92f));
		ImGui::PushStyleColor(ImGuiCol_Text,      ImVec4(0.78f, 0.78f, 0.78f, 0.92f));
		ImGui::PushStyleColor(ImGuiCol_Border,    ImVec4(0.14f, 0.14f, 0.14f, 0.92f));
		ImGui::PushStyleColor(ImGuiCol_FrameBg,   ImVec4(0.14f, 0.14f, 0.14f, 0.92f));

		ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.25f, 0.25f, 0.25f, 0.92f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.58f, 0.58f, 0.58f, 0.92f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.16f, 0.16f, 0.16f, 0.92f));

		ImGuiWindowFlags EditorUiFlags = ImGuiWindowFlags_NoTitleBar;
		if (EditorWinFixed)
			EditorUiFlags |= ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
		ImGui::Begin(w_name, (bool*)NULL, EditorUiFlags);
		{
			ImGui::SetNextItemWidth(142.0f);
			ImGui::InputInt("##IN_PAGE", &FramesViewPageLoc);

			size_t FramePagesMax = FramesDatasetIndex->size() / VIEW_ELEMENT_PAGES;
			// clamp pages_number max. frames.
			FramesViewPageLoc = (int)IM_MATHS_CLAMP((size_t)FramesViewPageLoc, 0, FramePagesMax);
			ImGui::SameLine();
			
			ImGui::SetNextItemWidth(142.0f);
			ImGui::InputInt("##IN_F_POS", &FrameViewInTemp);
			ImGui::SameLine();
			bool LocationFlag = ImGui::Button("Location");

			ImGui::SameLine();
			ImGui::Text("INFO: Pages Max: %u, Frames Max: %u, SmpPoints Number: %u", 
				FramePagesMax, 
				FramesDatasetIndex->size(), 
				PointsDatasetIndex->size()
			);
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Current: %u ", CurrentViewFrame + 1);

			ImGui::SameLine(); ImGui::SetNextItemWidth(128.0f);
			ImGui::InputFloat("SCALE", &ViewPlotValueScale);

			ImGui::SameLine(); ImGui::SetNextItemWidth(128.0f);
			if (ImGui::Button("SWITCH"))
				ViewPlotFlagSwitch = !ViewPlotFlagSwitch;

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.0f);
			// calc page => frames begin,end (offset,size).
			size_t F_PageBegin = FramesViewPageLoc * VIEW_ELEMENT_PAGES;
			size_t F_CountMax = 
				F_PageBegin + VIEW_ELEMENT_PAGES > FramesDatasetIndex->size() ? FramesDatasetIndex->size() : F_PageBegin + VIEW_ELEMENT_PAGES;

			ViewPlotValueScale = IM_MATHS_CLAMP(ViewPlotValueScale, 0.0001f, 2.0f);
			
			// draw memory usage plot.
			if (ViewPlotFlagSwitch) {
				float ValueMax1 = LimitMemoryUsage.x + (LimitMemoryUsage.y - LimitMemoryUsage.x) * ViewPlotValueScale;
				DrawLocationPlotLines("##MEM", "MEMORY USAGE",
					&FramesMemoryUsage, F_CountMax - F_PageBegin, F_PageBegin, ImVec2(LimitMemoryUsage.x, ValueMax1), ScrollxPosition
				);
			}
			else {
				// draw interval time plot.
				float ValueMax2 = LimitTimeInterval.x + (LimitTimeInterval.y - LimitTimeInterval.x) * ViewPlotValueScale;
				DrawLocationPlotLines("##FPS", "FRAME TIME",
					&FramesIntervalTime, F_CountMax - F_PageBegin, F_PageBegin, ImVec2(LimitTimeInterval.x, ValueMax2), ScrollxPosition
				);
			}

			const float ItemSpac2x = IMGUI_ITEM_SPAC * 2.0f;
			ImVec2 ChildWindowSize = ImGui::GetWindowSize() * ImVec2(1.0f, 0.28f) - ItemSpac2x;
			ImGui::BeginChild("@FRAMES_VIEW", ChildWindowSize);
			{
				ImGui::SetCursorPosY(IMGUI_ITEM_SPAC);
				ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.0f, 0.2f, 0.92f));

				// draw page_frames view.
				for (size_t i = F_PageBegin; i < F_CountMax; ++i) {
					if (i != F_PageBegin) ImGui::SameLine();

					// draw frames item.
					float ItemHeight = ImGui::GetWindowSize().y - ItemSpac2x;
					ImGui::BeginChild(to_string(i).c_str(), ImVec2(176.0f, ItemHeight), true, ImGuiWindowFlags_NoScrollWithMouse);
					
					ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "FrameCount %u", (*FramesDatasetIndex)[i].SmpFrameUnqiue);
					ImGui::TextColored(ImVec4(0.78f, 0.1f, 1.0f, 1.0f), "SamplePoints");

					size_t FramePoints = (*FramesDatasetIndex)[i].SmpPointDatasetSize / sizeof(FTDViewDataset::SrcDataSamplingPoint);
					ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.7f, 1.0f), "%u items", FramePoints);

					double ProcMemory = (double)(*FramesDatasetIndex)[i].SmpFrameProcessMemoryUsage / 1024.0 / 1024.0;
					ImGui::TextColored(ImVec4(0.78f, 0.1f, 1.0f, 1.0f), "Memory / Time");
					ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.7f, 1.0f), "%.4f mib", ProcMemory);
					ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.7f, 1.0f), "%.4f ms", (*FramesDatasetIndex)[i].SmpFrameClock);

					if (ImGui::IsWindowFocused()) {
						CurrentViewFrame = i;
						auto PointTemp = (*FramesDatasetIndex)[i];
						// frame =mapping=> points.
						FrameSamplePointsBegin = PointTemp.SmpPointDatasetOffset / sizeof(FTDViewDataset::SrcDataSamplingPoint);
						FrameSamplePointsEnd = FrameSamplePointsBegin + PointTemp.SmpPointDatasetSize / sizeof(FTDViewDataset::SrcDataSamplingPoint);
					}
					ImGui::EndChild();
				}
				ImGui::PopStyleColor(1);

				// frames location clamp.
				if (LocationFlag) {
					size_t RangeMax = F_CountMax - F_PageBegin;
					FrameViewInTemp = IM_MATHS_CLAMP(FrameViewInTemp, 0, (int)RangeMax);
					FrameViewScroll.x = (float)FrameViewInTemp / (float)RangeMax;
				}
				// calc_lerp, a += (b - a) * const * speed.
				FrameViewScroll.y += (FrameViewScroll.x - FrameViewScroll.y) * 0.038f * EditorLerpSpeed;
				// stop scroll(lerp).
				if (abs(FrameViewScroll.x - FrameViewScroll.y) > 0.0005f)
					ImGui::SetScrollX(ImGui::GetScrollMaxX() * FrameViewScroll.y);
				// scroll_pos: 0.0f - 1.0f
				if (ImGui::GetScrollMaxX() != 0.0f)
					ScrollxPosition = ImGui::GetScrollX() / ImGui::GetScrollMaxX();
			}
			ImGui::EndChild();

			DrawTotalMarkViews("##VIEW_TOTAL_MAKER", &FrameProcessingMarker, F_CountMax - F_PageBegin, F_PageBegin, ScrollxPosition);

			ImGui::SetNextItemWidth(142.0f);
			ImGui::InputInt("##IN_P_POS", &PointsViewPageLoc);

			size_t PointPagesMax = PointsDatasetIndex->size() / VIEW_ELEMENT_PAGES;
			// clamp pages_number max. points.
			PointsViewPageLoc = (int)IM_MATHS_CLAMP((size_t)PointsViewPageLoc, 0, PointPagesMax);

			// calc page => points begin,end (offset,size).
			size_t P_PageBegin = PointsViewPageLoc * VIEW_ELEMENT_PAGES;
			size_t P_CountMax =
				P_PageBegin + VIEW_ELEMENT_PAGES > PointsDatasetIndex->size() ? PointsDatasetIndex->size() : P_PageBegin + VIEW_ELEMENT_PAGES;

			ImGui::SameLine();
			if (ImGui::Button("LocationPages")) {
				FrameSamplePointsBegin = P_PageBegin;
				FrameSamplePointsEnd   = P_CountMax;
			}

			ImGui::SameLine();
			ImGui::Text(
				"INFO: Pages Max: %u, SmpPoints Begin: %u, End: %u, EditorFPS: %.2f LEVEL COLORS: ", 
				PointPagesMax, FrameSamplePointsBegin + 1, FrameSamplePointsEnd, ImGui::GetIO().Framerate
			);

			constexpr ImGuiColorEditFlags ColorButtonFlags =
				ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoTooltip;

			ImGui::SameLine(); ImGui::ColorButton("0", TagColorLv1, ColorButtonFlags, ImVec2(16.0f, 28.0f));
			ImGui::SameLine(); ImGui::ColorButton("0", TagColorLv2, ColorButtonFlags, ImVec2(16.0f, 28.0f));
			ImGui::SameLine(); ImGui::ColorButton("0", TagColorLv3, ColorButtonFlags, ImVec2(16.0f, 28.0f));
			ImGui::SameLine(); ImGui::ColorButton("0", TagColorLv4, ColorButtonFlags, ImVec2(16.0f, 28.0f));
			ImGui::SameLine(); ImGui::ColorButton("0", TagColorLv5, ColorButtonFlags, ImVec2(16.0f, 28.0f));

			ImGui::SameLine(); ImGui::Checkbox("Fixed Window", &EditorWinFixed);

			DrawColorsLevelMap("##VIEW_LEVEL", 
				PointsDatasetIndex, FrameSamplePointsEnd - FrameSamplePointsBegin, FrameSamplePointsBegin
			);
			
			ChildWindowSize = ImGui::GetWindowSize() * ImVec2(1.0f, 0.32f) - ImVec2(ItemSpac2x, IMGUI_ITEM_SPAC);
			ImGui::BeginChild("@POINTS_VIEW", ChildWindowSize);
			{
				ImGui::SetCursorPosY(IMGUI_ITEM_SPAC);
				ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.0f, 0.2f, 0.92f));

				// draw points element_view.
				for (size_t i = FrameSamplePointsBegin; i < FrameSamplePointsEnd; ++i) {
					if (i != FrameSamplePointsBegin) ImGui::SameLine();

					// draw sample_point item.
					float ItemHeight = ImGui::GetWindowSize().y - ItemSpac2x;
					ImGui::BeginChild(to_string(i).c_str(), ImVec2(176.0f, ItemHeight), true, ImGuiWindowFlags_NoScrollWithMouse);

					ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.7f, 1.0f), "ThreadID: %u", (*PointsDatasetIndex)[i].SmpPointTagThread);
					ImGui::TextColored(ImVec4(0.78f, 0.1f, 1.0f, 1.0f), "Context Timer:");
					ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.7f, 1.0f), "%.4f ms", (*PointsDatasetIndex)[i].SmpPointTimer);

					double OperMemory = (double)(*PointsDatasetIndex)[i].SmpPointMemOperBytes / 1024.0 / 1024.0;
					ImGui::TextColored(ImVec4(0.78f, 0.1f, 1.0f, 1.0f), "Memory Oper:");
					ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.7f, 1.0f), "%.4f mib", OperMemory);

					ImGui::TextColored(ImVec4(0.78f, 0.1f, 1.0f, 1.0f), "Sampler Pointer:");
					ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.7f, 1.0f), "0x%X", (*PointsDatasetIndex)[i].SmpPointTagPointer);

					// draw level_tag(rect_fill).
					RzBase::ListDrawRectangleFill(
						ImGui::GetCursorPos() + ImVec2(0.0f, 5.5f), ImVec2(ImGui::GetWindowSize().x - ItemSpac2x, 10.0f),
						LVTAG_COLOR_FIND((*PointsDatasetIndex)[i].SmpPointTagFilter)
					);

					// points tag_text draw.
					FTDViewDataset::id_count_tagging TextsCount = (*PointsDatasetIndex)[i].SmpPointTagText;
					if (ImGui::IsWindowHovered() && (bool)TextsCount) {
						ImGui::BeginTooltip();
						ImGui::Text("%u, %s", i, (*TextsDatasetIndex)[TextsCount].c_str());
						ImGui::EndTooltip();
					}
					ImGui::EndChild();
				}
				ImGui::PopStyleColor(1);
			}
			ImGui::EndChild();
			ImGui::Text("FTDebugEditor Graph: OPENGL,IMGUI By.RCSZ v20240818.");
		}
		ImGui::End();

		ImGui::PopStyleColor(8);
		ImGui::PopStyleVar(3);
	}
}