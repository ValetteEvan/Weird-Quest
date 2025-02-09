#include "langManager.h"

int lang_Count;

void lang_Init() {
	lang_LSentinel = calloc(1, sizeof(LangLanguage));
	if (!lang_LSentinel) return;
	lang_LSentinel->id = -1;
	lang_Count = 0;

	dirent* de = NULL;
	DIR* dr = opendir(PATH_LANGS);
	if (!dr) return;

	while ((de = readdir(dr)) != NULL) {
		if (strcmp(de->d_name, ".") && strcmp(de->d_name, "..")) {
			lang_LoadFile(de->d_name);
		}
	}
	closedir(dr);

	lang_CurrentID = 0;
}

void lang_LoadFile(char* _name) {
	if (!_name) return;

	LangLanguage* lang = calloc(1, sizeof(LangLanguage));
	strcpy(lang->name, _name);
	lang->keySentinel = calloc(1, sizeof(LangKey));

	char* fullpath = calloc(LANG_LEN_KEY + 1 + strlen(PATH_LANGS), sizeof(char));
	if (!fullpath) return;
	strcpy(fullpath, PATH_LANGS);
	strcat(fullpath, _name);
	FILE* f = fopen(fullpath, "rb");

	LangKey* nodeNew;
	char c;
	while (1) {
		c = getc(f);
		if (c == '\"') {
			nodeNew = calloc(1, sizeof(LangKey));
			if (nodeNew->key == NULL) return;
			int i = 0;
			c = getc(f);
			do {
				nodeNew->key[i] = c;
				i++;
				c = getc(f);
			} while (c != '\"');

			do { c = getc(f); } while (c != ':');

			do { c = getc(f); } while (c == ' ' || c == '\t');
			if (c != '\"') return;

			nodeNew->value = calloc(LANG_LEN_VAL, sizeof(char));
			if (nodeNew->value == NULL) return;
			i = 0;
			c = getc(f);
			do {
				if (c == '\\') {
					c = getc(f);
					switch (c) {
						case 'n': nodeNew->value[i] = '\n'; break;
						case 'r': nodeNew->value[i] = '\r'; break;
						case 't': nodeNew->value[i] = '\t'; break;
						default:  nodeNew->value[i] = '\\'; break;
					}
				}
				else nodeNew->value[i] = c;
				i++;
				c = getc(f);
			} while (c != '\"');
			do { c = getc(f); } while (c != ',' && c != '}' && c != EOF);
			lang_AddKey(lang, nodeNew);
			if (c == '}' || c == EOF) break;

			nodeNew = NULL;
		}
	}

	lang_AddLang(lang);
	fclose(f);
	lang_Count++;
}

void lang_AddLang(LangLanguage* _l) {
	_l->next = lang_LSentinel->next;
	lang_LSentinel->next = _l;
	_l->prev = lang_LSentinel;
	if (_l->next) _l->next->prev = _l;

	LangKey* itrK = _l->keySentinel->next;
	while (itrK != NULL) {
		if (!strcmp(itrK->key, "id")) {
			_l->id = atoi(itrK->value);
			return;
		}
		itrK = itrK->next;
	}
}

void lang_AddKey(LangLanguage* _l, LangKey* _k) {
	_k->next = _l->keySentinel->next;
	_l->keySentinel->next = _k;
	_k->prev = _l->keySentinel;
	if (_k->next) _k->next->prev = _k;
}

char* lang_GetFromKey(char* _key) {
	LangLanguage* itrL = lang_LSentinel->next;
	char* str = NULL;
	while (itrL != NULL) {
		if (itrL->id == lang_CurrentID) {
			LangKey* itrK = itrL->keySentinel->next;
			while (itrK != NULL) {
				if (!strcmp(itrK->key, _key)) {
					str = itrK->value;
				}
				itrK = itrK->next;
			}
			if (!str) return _key;
		}
		itrL = itrL->next;
	}
	return str ? str : _key;
}

void lang_SetLang(int _id) { lang_CurrentID = _id; }

char* lang_GetName(int _id) {
	LangLanguage* itrL = lang_LSentinel->next;
	while (itrL != NULL) {
		if (itrL->id == _id) {
			LangKey* itrK = itrL->keySentinel->next;
			while (itrK != NULL) {
				if (!strcmp(itrK->key, "language_name")) return itrK->value;
				itrK = itrK->next;
			}
		}
		itrL = itrL->next;
	}

	return "Unknown";
}

int lang_GetCount() { return lang_Count; }
int lang_GetCurrentID() { return lang_CurrentID; }

LangLanguage* lang_DestroyL(LangLanguage* _l) {
	LangLanguage* p = _l->prev;
	LangLanguage* n = _l->next;

	p->next = n;
	if (n) n->prev = p;
	free(_l);
	return n;
}

LangKey* lang_DestroyK(LangKey* _k) {
	LangKey* p = _k->prev;
	LangKey* n = _k->next;

	p->next = n;
	if (n) n->prev = p;
	free(_k);
	return n;
}

void lang_ClearBuffer() {
	LangLanguage* itrL = lang_LSentinel->next;
	while (itrL != NULL) {
		LangKey* itrK = itrL->keySentinel->next;
		while (itrK != NULL) itrK = lang_DestroyK(itrK);
		itrL = lang_DestroyL(itrL);
	}
}

void lang_UnitTest() {
	printf("Available languages: ");
	for (int i = 0; i < lang_GetCount(); i++)
		printf("%s ", lang_GetName(i));
	printf("\n");

	lang_SetLang(0);
	printf("%s: %s\n", lang_GetName(lang_GetCurrentID()), lang_GetFromKey("test_line"));
	lang_SetLang(1);
	printf("%s: %s\n", lang_GetName(lang_GetCurrentID()), lang_GetFromKey("test_line"));
	lang_SetLang(2);
	printf("%s: %s\n", lang_GetName(lang_GetCurrentID()), lang_GetFromKey("test_line"));
}