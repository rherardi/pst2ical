#if !defined(AFX_CONFIG_H__98F73F2A_7E4C_4D14_AF61_06664CED6C53__INCLUDED_)
#define AFX_CONFIG_H__98F73F2A_7E4C_4D14_AF61_06664CED6C53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define XMLCONFIG_BASEDN			_T("BaseDN")
#define XMLCONFIG_FILTER			_T("Filter")
#define XMLCONFIG_INPUTDIR			_T("InputDir")
#define XMLCONFIG_OUTPUTDIR			_T("OutputDir")
#define XMLCONFIG_LOGDIR			_T("LogDir")
#define XMLCONFIG_LOGSIZEWRAP		_T("LogSizeWrap")
#define XMLCONFIG_PSTFILEPWD		_T("PSTFilePassword")
#define XMLCONFIG_MAILHOST			_T("MailHost")
#define XMLCONFIG_DOMAIN			_T("Domain")
#define XMLCONFIG_SMTPADR			_T("SMTPAddressFile")
#define XMLCONFIG_SMTPPARTSSEP		_T("SMTPPartsSeparator")

#define PST_MASK_LASTNAME			_T("LastName")
#define PST_MASK_MIDDLENAME			_T("MiddleName")
#define PST_MASK_FIRSTNAME			_T("FirstName")
#define PST_MASK_SERVER				_T("Server")
#define PST_MASK_DOMAIN				_T("Domain")
#define PST_MASK_YEAR_AS_YYYY		_T("YearAsYYYY")
#define PST_MASK_MONTH_AS_MM		_T("MonthAsMM")
#define PST_MASK_DAY_AS_DD			_T("DayAsDD")
#define PST_MASK_SEP_UNDERLINE		_T("SeparatorUnderline")
#define PST_MASK_SEP_MINUS			_T("SeparatorMinus")
#define PST_MASK_SEP_SPACE			_T("SeparatorSpace")
#define PST_MASK_JOBNUMBER			_T("PurchaseOrderOrJobNumber")

#define PSTMASK_ENTRY_SELECTOR		_T("//pstMask/pstMaskItem")

#define SERVER_LOCAL				_T("Local")

#ifndef MAILBOX_FOLDERS_SELECTOR
#define MAILBOX_FOLDERS_SELECTOR	_T("//mailbox/folders")
#endif

#endif // !defined(AFX_CONFIG_H__98F73F2A_7E4C_4D14_AF61_06664CED6C53__INCLUDED_)
