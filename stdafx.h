// stdafx.h: ���������� ���� ��� ����������� ��������� ���������� ������
// ��� ���������� ������ ��� ����������� �������, ������� ����� ������������, ��
// �� ����� ����������
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <string>
#include <assert.h>

#define _in
#define _out

typedef wchar_t char_t;
typedef char_t *str_t;
typedef const char_t *cstr_t;

typedef std::wstring string_t;

// TODO: ���������� ����� ������ �� �������������� ���������, ����������� ��� ���������
#define OFILE_NAME		L"result.html"