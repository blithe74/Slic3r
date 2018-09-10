#include "PrintHostSendDialog.hpp"

#include <boost/filesystem/path.hpp>

#include <wx/frame.h>
#include <wx/event.h>
#include <wx/progdlg.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>

#include "slic3r/GUI/GUI.hpp"
#include "slic3r/GUI/MsgDialog.hpp"
#include "slic3r/GUI/AppConfig.hpp"


namespace fs = boost::filesystem;

namespace Slic3r {

static const std::string CONFIG_KEY_PATH  = "printhost_path";
static const std::string CONFIG_KEY_PRINT = "printhost_print";

PrintHostSendDialog::PrintHostSendDialog(const std::string &path) :
	MsgDialog(nullptr, _(L("Send G-Code to printer host")), _(L("Upload to Printer Host with the following filename:")), wxID_NONE),
	txt_filename(new wxTextCtrl(this, wxID_ANY)),
	box_print(new wxCheckBox(this, wxID_ANY, _(L("Start printing after upload"))))
{
	auto *label_dir_hint = new wxStaticText(this, wxID_ANY, _(L("Use forward slashes ( / ) as a directory separator if needed.")));
	label_dir_hint->Wrap(CONTENT_WIDTH);

	content_sizer->Add(txt_filename, 0, wxEXPAND);
	content_sizer->Add(label_dir_hint);
	content_sizer->AddSpacer(VERT_SPACING);
	content_sizer->Add(box_print, 0, wxBOTTOM, 2*VERT_SPACING);

	btn_sizer->Add(CreateStdDialogButtonSizer(wxOK | wxCANCEL));

	fs::path fspath(path);
	fs::path filename = fspath.filename();
#ifdef _WIN32
	wxString wxfilename(filename.wstring());
	wxString stem(filename.stem().wstring());
#else
	wxString wxfilename(filename.string());
	wxString stem(filename.stem().string());
#endif

	auto hostpath = wxString::FromUTF8(GUI::get_app_config()->get(CONFIG_KEY_PATH).c_str());
	if (hostpath.Length() > 0 && hostpath[hostpath.Length() - 1] != '/') {
		hostpath += '/';
	}

	txt_filename->SetValue(hostpath + wxfilename);
	txt_filename->SetInsertionPoint(0);
	txt_filename->SetFocus();

	bool print = GUI::get_app_config()->get(CONFIG_KEY_PRINT) == "1";
	box_print->SetValue(print);

	Fit();

	const auto hostpath_len = hostpath.Length();
	const auto stem_len = stem.Length();
	Bind(wxEVT_SHOW, [=](const wxShowEvent &event) {
		txt_filename->SetSelection(hostpath_len, hostpath_len + stem_len);
	});
}

std::string PrintHostSendDialog::remote_path() const
{
	return GUI::into_u8(txt_filename->GetValue());
}

bool PrintHostSendDialog::print() const
{
	return box_print->GetValue();
}

void PrintHostSendDialog::EndModal(int ret)
{
	if (ret == wxID_OK) {
		// Persist path and print settings
		wxString path = txt_filename->GetValue();
		int last_slash = path.Find('/', true);
		if (last_slash != wxNOT_FOUND) {
			path = path.SubString(0, last_slash);
			GUI::get_app_config()->set(CONFIG_KEY_PATH, GUI::into_u8(path));
		}

		bool print = box_print->GetValue();
		GUI::get_app_config()->set(CONFIG_KEY_PRINT, print ? "1" : "0");
	}

	MsgDialog::EndModal(ret);
}

}
