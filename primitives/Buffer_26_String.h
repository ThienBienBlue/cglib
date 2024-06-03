struct Buffer_26_String
{
	int length;
	struct String buffer[26];
};

struct Buffer_26_String Buffer_26_String_init(struct String zero);

bool Buffer_26_String_put(struct Buffer_26_String* self, int idx, struct String item);

bool Buffer_26_String_push(struct Buffer_26_String* self, struct String item);

bool Buffer_26_String_pop(struct Buffer_26_String* self);
