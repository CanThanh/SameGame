#pragma once
#include "afxwin.h"


// COptionsDialog dialog

class COptionsDialog : public CDialog
{
	DECLARE_DYNAMIC(COptionsDialog)

public:
	COptionsDialog(bool bRowColumn,CWnd* pParent = NULL);   // standard constructor
	virtual ~COptionsDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_ctrlStaticText1;
	CStatic m_ctrlStaticText2;
	int m_nValue1;
	int m_nValue2;
	afx_msg void OnBnClickedButtonDefault();
	virtual BOOL OnInitDialog();
	private:
	/*  Is this dialog for row/column (true) or width/height (false)? */
	bool m_bRowColumnDialog;
};
