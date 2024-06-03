struct Buffer<NT>
{
	int length;
	T buffer[N];
};

struct Buffer<NT> Buffer<NT>_init(T zero);

bool Buffer<NT>_put(struct Buffer<NT>* self, int idx, T item);

bool Buffer<NT>_push(struct Buffer<NT>* self, T item);

bool Buffer<NT>_pop(struct Buffer<NT>* self);
