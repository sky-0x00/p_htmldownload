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

typedef unsigned char byte_t;

typedef std::wstring string_t;

//struct strinfo
//{
//	cstr_t data;
//	unsigned length;
//};
//typedef struct strinfo strinfo_t;

// TODO: ���������� ����� ������ �� �������������� ���������, ����������� ��� ���������
#define OFILE_NAME		L"result.html"