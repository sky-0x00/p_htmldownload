// stdafx.h: включаемый файл дл€ стандартных системных включаемых файлов
// или включаемых файлов дл€ конкретного проекта, которые часто используютс€, но
// не часто измен€ютс€
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

// TODO: ”становите здесь ссылки на дополнительные заголовки, требующиес€ дл€ программы
#define OFILE_NAME		L"result.html"