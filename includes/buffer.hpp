#ifndef BUFFER_HPP
#define BUFFER_HPP

class Buffer
{
	private:
		unsigned int ID;
		unsigned int size;
		unsigned int index;

	public:
		Buffer(unsigned int index, unsigned int size);
		~Buffer();

		void CreateBuffer();
		void BindBufferBase(unsigned int bindIndex);
		void BindBuffer();
		void *GetPointer();
		void UnMapBuffer();
};

#endif