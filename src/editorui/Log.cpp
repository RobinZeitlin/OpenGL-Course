#include "Log.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <sstream>
#include <iostream>
#include <mutex>

#include "../Game.h"

class LogStreamBuffer : public std::streambuf {
public:
    LogStreamBuffer(std::ostream& stream) : originalStream(stream) {
        originalBuffer = stream.rdbuf(this);
    }
    ~LogStreamBuffer() {
        originalStream.rdbuf(originalBuffer);
    }

protected:
    int overflow(int c) override {
        if (c != EOF) {
            std::lock_guard<std::mutex> lock(mutex);
            logBuffer += static_cast<char>(c);
            if (c == '\n') {
                logEntries.insert(logEntries.begin(), logBuffer);
                logBuffer.clear();
            }
        }
        return c;
    }

public:
    static std::vector<std::string> logEntries;
    static std::mutex mutex;

private:
    std::ostream& originalStream;
    std::streambuf* originalBuffer;
    std::string logBuffer;
};

std::vector<std::string> LogStreamBuffer::logEntries;
std::mutex LogStreamBuffer::mutex;

static LogStreamBuffer logStreamBuffer(std::cout);

void Log::draw(Game* game)
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;

    if (ImGui::Begin("UtilWindow", nullptr, window_flags))
    {
        if (ImGui::BeginTabBar("Util"))
        {
            if (ImGui::BeginTabItem("Log"))
            {
                ImGui::BeginChild("LogScroll", ImVec2(0, 300), true, ImGuiWindowFlags_HorizontalScrollbar);
                std::lock_guard<std::mutex> lock(LogStreamBuffer::mutex);
                for (const auto& entry : LogStreamBuffer::logEntries)
                {
                    ImGui::TextUnformatted(entry.c_str());
                }
                if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                {
                    ImGui::SetScrollHereY(1.0f);
                }
                ImGui::EndChild();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();
    }
}
