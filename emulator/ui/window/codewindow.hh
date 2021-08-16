#ifndef EMULATOR_CODEWINDOW_HH
#define EMULATOR_CODEWINDOW_HH

#include <functional>
#include "../window.hh"
#include "../../compiler/debug.hh"
#include "../model/codemodel.hh"
#include "fileselectwindow.hh"

class CodeWindow : public Window {
public:
    CodeWindow(Emulator& emulator, FileSelectWindow& file_select_window)
        : Window(emulator), file_select_window_(file_select_window) {}
    
    void draw() override;
    
    std::string name() const override { return "code"; }
    
    static std::string window_title() { return "Code"; }
    
    void set_code_model(CodeModel& code_model) { code_model_ = &code_model; }
    
private:
    void draw_buttons();
    void draw_code();
    void draw_footer();
    
    FileSelectWindow& file_select_window_;
    
    CodeModel* code_model_ = nullptr;
    bool scroll_to_pc_ = true;
    std::optional<size_t> show_this_line_on_next_frame {};
};

#endif
