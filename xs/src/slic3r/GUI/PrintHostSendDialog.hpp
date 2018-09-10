#ifndef slic3r_PrintHostSendDialog_hpp_
#define slic3r_PrintHostSendDialog_hpp_

#include <string>

#include "slic3r/GUI/MsgDialog.hpp"

class wxTextCtrl;
class wxCheckBox;


namespace Slic3r {

class PrintHostSendDialog : public GUI::MsgDialog
{
private:
	wxTextCtrl *txt_filename;
	wxCheckBox *box_print;
	bool can_start_print;

public:
	PrintHostSendDialog(const std::string &path);   // path should be in utf-8
	std::string remote_path() const;                // returns path in utf-8
	bool print() const;                             // whether printing should be started

	virtual void EndModal(int ret);
};

}

#endif
