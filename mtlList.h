#ifndef MTL_LIST_H_INCLUDED__
#define MTL_LIST_H_INCLUDED__

namespace mtl
{

	template < typename type_t > class List;

	template < typename type_t >
	class Node
	{
		friend class List<type_t>;
	private:
		mtl::List<type_t>	*m_parent;
		mtl::Node<type_t>	*m_next;
		mtl::Node<type_t>	*m_prev;
	private:
							Node(const mtl::Node<type_t>&) {}
		mtl::Node<type_t>	&operator=(const mtl::Node<type_t>&) { return *this; }
							Node(const type_t &p_value, mtl::List<type_t> *p_parent, mtl::Node<type_t> *p_next, mtl::Node<type_t> *p_prev);
	public:
		type_t value;
	public:
		inline							Node( void );
		inline explicit					Node(const type_t &p_value);
		inline							~Node( void );
		inline const mtl::List<type_t>	*GetParent( void ) const;
		inline mtl::List<type_t>		*GetParent( void );
		inline const mtl::Node<type_t>	*GetNext( void ) const;
		inline mtl::Node<type_t>		*GetNext( void );
		inline const mtl::Node<type_t>	*GetPrev( void ) const;
		inline mtl::Node<type_t>		*GetPrev( void );
	};

	template < typename type_t >
	class List
	{
	private:
		mtl::Node<type_t>	*m_front;
		mtl::Node<type_t>	*m_back;
		int					m_size;
	private:
		List(const mtl::List<type_t>&) {}
		mtl::List<type_t> &operator=(const mtl::List<type_t>&) { return *this; }
	public:
		inline							List( void );
		inline							~List( void );
		void							PushBack(const type_t &p_value);
		void							PushFront(const type_t &p_value);
		void							PopBack( void );
		void							PopFront( void );
		mtl::Node<type_t>				*Insert(const type_t &p_value, mtl::Node<type_t> *p_node);
		mtl::Node<type_t>				*Insert(mtl::List<type_t> &p_list, mtl::Node<type_t> *p_node);
		mtl::Node<type_t>				*Remove(mtl::Node<type_t> *p_node);
		void							Split(mtl::Node<type_t> *p_begin, int p_num, mtl::List<type_t> &p_out);
		void							Swap(mtl::Node<type_t> *p_a, mtl::Node<type_t> *p_b);
		void							Promote(mtl::Node<type_t> *p_node);
		void							Demote(mtl::Node<type_t> *p_node);
		void							Copy(const mtl::List<type_t> &p_list);
		void							Free( void );
		inline int						GetSize( void ) const;
		inline mtl::Node<type_t>		*GetBack( void );
		inline const mtl::Node<type_t>	*GetBack( void ) const;
		inline mtl::Node<type_t>		*GetFront( void );
		inline const mtl::Node<type_t>	*GetFront( void ) const;
	};

}

template < typename type_t >
mtl::Node<type_t>::Node(const type_t &p_value, mtl::List<type_t> *p_parent, mtl::Node<type_t> *p_next, mtl::Node<type_t> *p_prev) :
m_parent(p_parent), m_next(p_next), m_prev(p_prev), value(p_value)
{
	if (m_prev != 0) { m_prev->m_next = this; }
	if (m_next != 0) { m_next->m_prev = this; }
}

template < typename type_t >
mtl::Node<type_t>::Node( void ) :
m_parent(0), m_next(0), m_prev(0)
{}

template < typename type_t >
mtl::Node<type_t>::Node(const type_t &p_value) :
m_parent(0), m_next(0), m_prev(0), value(p_value)
{}

template < typename type_t >
mtl::Node<type_t>::~Node( void )
{
	m_parent = 0;
	delete m_next;
}

template < typename type_t >
const mtl::List<type_t> *mtl::Node<type_t>::GetParent( void ) const
{
	return m_parent;
}

template < typename type_t >
mtl::List<type_t> *mtl::Node<type_t>::GetParent( void )
{
	return m_parent;
}

template < typename type_t >
const mtl::Node<type_t> *mtl::Node<type_t>::GetNext( void ) const
{
	return m_next;
}

template < typename type_t >
typename mtl::Node<type_t> *mtl::Node<type_t>::GetNext( void )
{
	return m_next;
}

template < typename type_t >
const mtl::Node<type_t> *mtl::Node<type_t>::GetPrev( void ) const
{
	return m_prev;
}

template < typename type_t >
typename mtl::Node<type_t> *mtl::Node<type_t>::GetPrev( void )
{
	return m_prev;
}

template < typename type_t >
mtl::List<type_t>::List( void ) :
m_front(0), m_back(0), m_size(0)
{}

template < typename type_t >
mtl::List<type_t>::~List( void )
{
	delete m_front;
}

template < typename type_t >
void mtl::List<type_t>::PushBack(const type_t &p_value)
{
	mtl::Node<type_t> *node = new mtl::Node<type_t>(p_value, this, 0, m_back);
	m_back = node;
	if (m_front == 0) { m_front = m_back; }
	++m_size;
}

template < typename type_t >
void mtl::List<type_t>::PushFront(const type_t &p_value)
{
	mtl::Node<type_t> *node = new mtl::Node<type_t>(p_value, this, m_front, 0);
	m_front = node;
	if (m_back == 0) { m_back = m_front; }
	++m_size;
}

template < typename type_t >
void mtl::List<type_t>::PopBack( void )
{
	mtl::Node<type_t> *node = m_back;
	m_back = m_back->m_prev;
	if (m_back == 0) { m_front = 0; }
	node->m_next = 0;
	delete node;
	--m_size;
}

template < typename type_t >
void mtl::List<type_t>::PopFront( void )
{
	mtl::Node<type_t> *node = m_front;
	m_front = m_front->m_next;
	if (m_front == 0) { m_back = 0; }
	node->m_next = 0;
	delete node;
	--m_size;
}

template < typename type_t >
mtl::Node<type_t> *mtl::List<type_t>::Insert(const type_t &p_value, mtl::Node<type_t> *p_node)
{
	if (p_node == 0) {
		PushBack(p_value);
		return m_back;
	} else if (p_node->m_parent != this) {
		return 0;
	} else if (p_node == m_front) {
		PushFront(p_value);
		return m_front;
	}
	mtl::Node<type_t> *node = new mtl::Node<type_t>(p_value, this, p_node, p_node->m_prev);
	++m_size;
	return p_node->m_prev;
}

template < typename type_t >
mtl::Node<type_t> *mtl::List<type_t>::Insert(mtl::List<type_t> &p_list, mtl::Node<type_t> *p_node)
{
	mtl::Node<type_t> *retNode = p_list.m_front;
	if (p_node == 0) {
		if (m_front == 0) {
			m_front = p_list->m_front;
			m_back = p_list->m_back;
		} else {
			m_back->m_next = p_list.m_front;
			m_back = p_list.m_back;
		}
	} else if (p_node->m_parent != this) {
		return 0;
	} else if (p_node == m_front) {
		m_front->m_prev = p_list.m_back;
		p_list.m_back->m_next = m_front;
		m_front = p_list.m_front;
	} else {
		p_node->m_next = p_list.m_front;
		p_list.m_front->m_prev = p_node;
		p_list.m_back->m_next = p_node->m_next;
	}
	m_size += p_list.m_size;
	p_list.m_front = p_list->m_back = 0;
	p_list.m_size = 0;
	return retNode;
}

template < typename type_t >
mtl::Node<type_t> *mtl::List<type_t>::Remove(mtl::Node<type_t> *p_node)
{
	if (p_node->m_parent != this) { return 0; }
	if (p_node == m_back) { m_back = p_node->m_prev; }
	mtl::Node<type_t> *next = p_node->m_next;
	next->m_prev = p_node->m_prev;
	if (next->m_prev != 0) { next->m_prev->m_next = next; }
	else { m_front = p_node->m_next; }
	p_node->m_next = 0;
	delete p_node;
	--m_size;
	return next;
}

template < typename type_t >
void mtl::List<type_t>::Split(mtl::Node<type_t> *p_begin, int p_num, mtl::List<type_t> &p_out)
{
	if (p_num <= 0 || p_begin->m_parent != this || &p_out == this) { return; }

	mtl::Node<type_t> *end = p_begin;
	end->m_parent = &p_out;
	for (int i = 1; i < p_num; ++i) {
		end = end->GetNext();
		end->m_parent = &p_out;
	}
	
	delete p_out.m_front;

	p_out.m_front = p_begin;
	p_out.m_back = end;
	if (p_begin == m_front) { m_front = end->m_next; }
	if (end == m_back) { m_back = p_begin->m_prev; }
	
	if (p_begin->m_prev != 0) { p_begin->m_prev->m_next = end->m_next; }
	p_begin->m_prev = 0;
	if (end->m_next != 0) { end->m_next->m_prev = p_begin->m_prev; }
	end->m_next = 0;

	p_out.m_size = p_num;
	m_size -= p_num;
}

template < typename type_t >
void mtl::List<type_t>::Swap(mtl::Node<type_t> *p_a, mtl::Node<type_t> *p_b)
{
	if (p_a->m_parent != this || p_b->m_parent != this) { return; }

	mtl::Node<type_t> *front = m_front;
	mtl::Node<type_t> *back = m_back;
	if (p_a == m_front)	{ front = p_b; }
	if (p_a == m_back)	{ back = p_b; }
	if (p_b == m_front)	{ front = p_a; }
	if (p_b == m_back)	{ back = p_a; }
	m_front = front;
	m_back = back;
	mtl::Node<type_t> *tempNext = p_a->m_next;
	mtl::Node<type_t> *tempPrev = p_a->m_prev;
	p_a->m_next = p_b->m_next;
	p_a->m_prev = p_b->m_prev;
	p_b->m_next = tempNext;
	p_b->m_prev = tempPrev;
	if (p_a->m_next != 0) { p_a->m_next->m_prev = p_a; }
	if (p_a->m_prev != 0) { p_a->m_prev->m_next = p_a; }
	if (p_b->m_next != 0) { p_b->m_next->m_prev = p_b; }
	if (p_b->m_prev != 0) { p_b->m_prev->m_next = p_b; }
}

template < typename type_t >
void mtl::List<type_t>::Promote(mtl::Node<type_t> *p_node)
{
	if (p_node->m_parent != this) { return; }
	if (p_node->m_next != 0) { p_node->m_next->m_prev = p_node->m_prev; }
	if (p_node->m_prev != 0) { p_node->m_prev->m_next = p_node->m_next; }
	m_front->m_prev = p_node;
	p_node->m_next = m_front;
	p_node->m_prev = 0;
	m_front = p_node;
}

template < typename type_t >
void mtl::List<type_t>::Demote(mtl::Node<type_t> *p_node)
{
	if (p_node->m_parent != this) { return; }
	if (p_node->m_next != 0) { p_node->m_next->m_prev = p_node->m_prev; }
	if (p_node->m_prev != 0) { p_node->m_prev->m_next = p_node->m_next; }
	m_back->m_next = p_node;
	p_node->m_next = 0;
	p_node->m_prev = m_back;
	m_back = p_node;
}

template < typename type_t >
void mtl::List<type_t>::Copy(const mtl::List<type_t> &p_list)
{
	if (this != &p_list) {
		delete m_front;
		m_size = p_list.m_size;
		mtl::Node<type_t> *src = p_list.m_front;
		if (src != 0) {
			m_back = m_front = new mtl::Node<type_t>(src->value, this, 0, 0);
			src = src->m_next;
			while (src != 0) {
				m_back = new mtl::Node<type_t>(src->value, this, 0, m_back);
				src = src->m_next;
			}
		}
	}
}

template < typename type_t >
void mtl::List<type_t>::Free( void )
{
	delete m_front;
	m_front = m_back = 0;
	m_size = 0;
}

template < typename type_t >
int mtl::List<type_t>::GetSize( void ) const
{
	return m_size;
}

template < typename type_t>
mtl::Node<type_t> *mtl::List<type_t>::GetBack( void )
{
	return m_back;
}

template < typename type_t>
const mtl::Node<type_t> *mtl::List<type_t>::GetBack( void ) const
{
	return m_back;
}

template < typename type_t>
mtl::Node<type_t> *mtl::List<type_t>::GetFront( void )
{
	return m_front;
}

template < typename type_t>
const mtl::Node<type_t> *mtl::List<type_t>::GetFront( void ) const
{
	return m_front;
}

#endif
