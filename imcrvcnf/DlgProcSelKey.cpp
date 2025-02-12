﻿
#include "configxml.h"
#include "imcrvcnf.h"
#include "resource.h"

static LPCWSTR listSelKey[MAX_SELKEY_C] =
{L"ＡAa", L"ＳSs", L"ＤDd", L"ＦFf", L"ＪJj", L"ＫKk", L"ＬLl", L"ＧGg", L"ＨHh"};

INT_PTR CALLBACK DlgProcSelKey(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hWndListView;
	LV_COLUMNW lvc;
	LVITEMW item;
	LPNMLISTVIEW pListView;
	int index;
	WCHAR num[2 + 1] = {};
	WCHAR key[4 + 1] = {};
	std::wstring strxmlval;
	WCHAR text[16] = {};

	switch (message)
	{
	case WM_INITDIALOG:
		hWndListView = GetDlgItem(hDlg, IDC_LIST_SELKEY);
		ListView_SetExtendedListViewStyle(hWndListView, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.fmt = LVCFMT_CENTER;
		lvc.pszText = text;

		lvc.iSubItem = 0;
		lvc.cx = GetScaledSizeX(hDlg, 60);
		wcsncpy_s(text, L"数字", _TRUNCATE);
		ListView_InsertColumn(hWndListView, 0, &lvc);
		lvc.iSubItem = 1;
		lvc.cx = GetScaledSizeX(hDlg, 60);
		wcsncpy_s(text, L"表示", _TRUNCATE);
		ListView_InsertColumn(hWndListView, 1, &lvc);
		lvc.iSubItem = 2;
		lvc.cx = GetScaledSizeX(hDlg, 60);
		wcsncpy_s(text, L"予備1", _TRUNCATE);
		ListView_InsertColumn(hWndListView, 2, &lvc);
		lvc.iSubItem = 3;
		lvc.cx = GetScaledSizeX(hDlg, 60);
		wcsncpy_s(text, L"予備2", _TRUNCATE);
		ListView_InsertColumn(hWndListView, 3, &lvc);

		SetDlgItemTextW(hDlg, IDC_EDIT_SELKEY_DISP, L"");
		SetDlgItemTextW(hDlg, IDC_EDIT_SELKEY_SPARE1, L"");
		SetDlgItemTextW(hDlg, IDC_EDIT_SELKEY_SPARE2, L"");

		hWndListView = GetDlgItem(hDlg, IDC_LIST_SELKEY);
		for (index = 0; index < MAX_SELKEY_C; index++)
		{
			_snwprintf_s(num, _TRUNCATE, L"%d", index + 1);

			item.mask = LVIF_TEXT;
			item.pszText = num;
			item.iItem = index;
			item.iSubItem = 0;
			ListView_InsertItem(hWndListView, &item);

			ZeroMemory(key, sizeof(key));
			ReadValue(pathconfigxml, SectionSelKey, num, strxmlval);
			if (strxmlval.empty()) strxmlval = listSelKey[index];
			wcsncpy_s(key, strxmlval.c_str(), _TRUNCATE);

			int sp = IS_SURROGATE_PAIR(key[0], key[1]) ? 1 : 0;

			num[0] = key[0];
			num[0 + sp] = key[sp];
			num[1 + sp] = L'\0';
			item.pszText = num;
			item.iItem = index;
			item.iSubItem = 1;
			ListView_SetItem(hWndListView, &item);
			num[0] = key[1 + sp];
			num[1] = L'\0';
			item.pszText = num;
			item.iItem = index;
			item.iSubItem = 2;
			ListView_SetItem(hWndListView, &item);
			num[0] = key[2 + sp];
			num[1] = L'\0';
			item.pszText = num;
			item.iItem = index;
			item.iSubItem = 3;
			ListView_SetItem(hWndListView, &item);
		}
		return TRUE;

	case WM_DPICHANGED_AFTERPARENT:
		hWndListView = GetDlgItem(hDlg, IDC_LIST_SELKEY);

		ListView_SetColumnWidth(hWndListView, 0, GetScaledSizeX(hDlg, 60));
		ListView_SetColumnWidth(hWndListView, 1, GetScaledSizeX(hDlg, 60));
		ListView_SetColumnWidth(hWndListView, 2, GetScaledSizeX(hDlg, 60));
		ListView_SetColumnWidth(hWndListView, 3, GetScaledSizeX(hDlg, 60));

		return TRUE;

	case WM_COMMAND:
		hWndListView = GetDlgItem(hDlg, IDC_LIST_SELKEY);
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_SELKEY_W:
			index = ListView_GetNextItem(hWndListView, -1, LVNI_SELECTED);
			if (index >= 0)
			{
				PropSheet_Changed(GetParent(hDlg), hDlg);

				GetDlgItemTextW(hDlg, IDC_EDIT_SELKEY_DISP, num, _countof(num));
				if (num[0] == L'\0')
				{
					break;
				}
				int sp = IS_SURROGATE_PAIR(num[0], num[1]) ? 1 : 0;
				num[1 + sp] = L'\0';
				SetDlgItemTextW(hDlg, IDC_EDIT_SELKEY_DISP, num);
				ListView_SetItemText(hWndListView, index, 1, num);
				GetDlgItemTextW(hDlg, IDC_EDIT_SELKEY_SPARE1, num, _countof(num));
				num[1] = L'\0';
				SetDlgItemTextW(hDlg, IDC_EDIT_SELKEY_SPARE1, num);
				ListView_SetItemText(hWndListView, index, 2, num);
				GetDlgItemTextW(hDlg, IDC_EDIT_SELKEY_SPARE2, num, _countof(num));
				num[1] = L'\0';
				SetDlgItemTextW(hDlg, IDC_EDIT_SELKEY_SPARE2, num);
				ListView_SetItemText(hWndListView, index, 3, num);

				return TRUE;
			}
			break;
		default:
			break;
		}
		break;

	case WM_NOTIFY:
		if (lParam == NULL) break;
		switch (((LPNMHDR)lParam)->code)
		{
		case LVN_ITEMCHANGED:
			pListView = (LPNMLISTVIEW)((LPNMHDR)lParam);
			if (pListView->uChanged & LVIF_STATE)
			{
				hWndListView = ((LPNMHDR)lParam)->hwndFrom;
				index = ListView_GetNextItem(hWndListView, -1, LVNI_SELECTED);
				if (index == -1)
				{
					SetDlgItemTextW(hDlg, IDC_EDIT_SELKEY_DISP, L"");
					SetDlgItemTextW(hDlg, IDC_EDIT_SELKEY_SPARE1, L"");
					SetDlgItemTextW(hDlg, IDC_EDIT_SELKEY_SPARE2, L"");
				}
				else
				{
					ListView_GetItemText(hWndListView, index, 1, num, _countof(num));
					SetDlgItemTextW(hDlg, IDC_EDIT_SELKEY_DISP, num);
					ListView_GetItemText(hWndListView, index, 2, num, _countof(num));
					SetDlgItemTextW(hDlg, IDC_EDIT_SELKEY_SPARE1, num);
					ListView_GetItemText(hWndListView, index, 3, num, _countof(num));
					SetDlgItemTextW(hDlg, IDC_EDIT_SELKEY_SPARE2, num);
				}
				return TRUE;
			}
			break;

		default:
			break;
		}
		break;

	default:
		break;
	}
	return FALSE;
}

void SaveSelKey(IXmlWriter *pWriter, HWND hDlg)
{
	HWND hWndListView;
	int index;
	WCHAR num[2 + 1] = {};
	WCHAR key[4 + 1] = {};

	hWndListView = GetDlgItem(hDlg, IDC_LIST_SELKEY);
	for (index = 0; index < MAX_SELKEY_C; index++)
	{
		ListView_GetItemText(hWndListView, index, 1, num, _countof(num));
		int sp = IS_SURROGATE_PAIR(num[0], num[1]) ? 1 : 0;
		key[0] = num[0];
		key[0 + sp] = num[0 + sp];
		ListView_GetItemText(hWndListView, index, 2, num, _countof(num));
		key[1 + sp] = num[0];
		ListView_GetItemText(hWndListView, index, 3, num, _countof(num));
		key[2 + sp] = num[0];
		key[3 + sp] = L'\0';

		_snwprintf_s(num, _TRUNCATE, L"%d", index + 1);
		WriterKey(pWriter, num, key);
	}
}
