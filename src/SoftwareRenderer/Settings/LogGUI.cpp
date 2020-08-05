#include "LogGUI.h"

LogGUI::LogGUI() : m_bScrollBottom(true)
{
}

void LogGUI::Log(const char* format, ...)
{
  int iOldSize = m_logBuffer.size();
  va_list args;

  va_start(args, format);
  m_logBuffer.appendfv(format, args);
  va_end(args);

  for (int newSize = m_logBuffer.size(); iOldSize < newSize; iOldSize++)
  {
    if (m_logBuffer[iOldSize] == '\n')
    {
      m_lineOffsets.push_back(iOldSize);
    }
  }
  m_bScrollBottom = true;
}

void LogGUI::Render(const char* title, bool* pOpened /*= NULL*/)
{
  ImGui::SetNextWindowSize(ImVec2(500, 300));
  ImGui::Begin(title, pOpened);
  if (ImGui::Button("Clear"))
  {
    Clear();
  }

  ImGui::SameLine();
  bool copy = ImGui::Button("Copy");
  ImGui::SameLine();
  m_filter.Draw("Filter", -100.0f);

  ImGui::Separator();
  ImGui::BeginChild("scrolling");
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
  if (copy)
  {
    ImGui::LogToClipboard();
  }

  if (m_filter.IsActive())
  {
    const char* bufferBegin = m_logBuffer.begin();
    const char* line = bufferBegin;
    for (int lineNo = 0; line != NULL; lineNo++)
    {
      const char* lineEnd = (lineNo < m_lineOffsets.Size) ? bufferBegin + m_lineOffsets[lineNo] : NULL;
      if (m_filter.PassFilter(line, lineEnd))
      {
        ImGui::TextUnformatted(line, lineEnd);
      }

      line = lineEnd && lineEnd[1] ? lineEnd + 1 : NULL;
    }
  }
  else
  {
    ImGui::TextUnformatted(m_logBuffer.begin());
  }

  if (m_bScrollBottom)
  {
    ImGui::SetScrollHereY(1.0f);
  }

  m_bScrollBottom = false;
  ImGui::PopStyleVar();
  ImGui::EndChild();
  ImGui::End();
}

void LogGUI::Clear()
{
  m_logBuffer.clear();
  m_lineOffsets.clear();
}
