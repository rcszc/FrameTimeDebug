// imrcsz_control_base. RCSZ.

#ifndef __IMRCSZ_CONTROL_BASE_H
#define __IMRCSZ_CONTROL_BASE_H
#include <vector>

#define IMRCSZ_CONTROL_BASE_MATHS
#include "imrcsz_control_base_ms.h"

namespace RzBase {
	// color(r,g,b,a): value => r,g,b ahpla => a.
	ImVec4 ColorGrayscaleScale(const ImVec4& color, float value, float ahpla = 0.0f);
	float ItemCenteredCalcX(float width);
	float ItemCenteredCalcY(float height);

	void ListDrawLine(const ImVec2& point0, const ImVec2& point1, const ImVec4& color, float width);
	void ListDrawText(const ImVec2& position, const ImVec4& color, const char* text, ...);

	void ListDrawRectangle    (const ImVec2& position, const ImVec2& size, const ImVec4& color, float wline = 3.2f);
	void ListDrawRectangleFill(const ImVec2& position, const ImVec2& size, const ImVec4& color);
	void ListDrawCircle       (const ImVec2& position, float size, const ImVec4& color, float wline = 3.2f);
	void ListDrawCircleFill   (const ImVec2& position, float size, const ImVec4& color);
	void ListDrawTriangle     (const ImVec2& position, const ImVec2& offset1, const ImVec2& offset2, const ImVec4& color, float wline = 3.2f);
	void ListDrawTriangleFill (const ImVec2& position, const ImVec2& offset1, const ImVec2& offset2, const ImVec4& color);

	void ListDrawRoundImage(const ImVec2& position, const ImVec2& size, ImTextureID texture);

	void ListDrawPolyLine(std::vector<ImVec2>* vertices, const ImVec4& color, float wline = 3.2f);
	void ListDrawPolyFilled(std::vector<ImVec2>* vertices, const ImVec4& color);

	namespace RzAnim {
#define ANIM_STD_STEP_BUTTON 0.078f

		class ButtonAnim {
		protected:
			ImVec4 ButtonAnimColor = {};
			ImVec2 ButtonAnimSize  = {};

			ImVec4 AnimStatColor[3] = {};
			ImVec2 AnimStatSize[3]  = {};
		public:
			bool DrawButton(const char* name, float speed = 1.0f);

			void SetButtonStatColor(const ImVec4& normal, const ImVec4& hover, const ImVec4& active);
			void SetButtonStatSize(const ImVec2& normal, const ImVec2& hover, const ImVec2& active);
		};
	}
}

#endif