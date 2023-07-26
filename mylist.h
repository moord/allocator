#pragma once

#include <iostream>
#include <memory>


template< typename T,  typename A = std::allocator<T> >
class my_list {
private:
    // Объявление структуры узла для использования в классе Iterator
    struct node;

    typename std::allocator_traits<A>::template rebind_alloc<node> alloc;

    using traits_node = std::allocator_traits<decltype(alloc)>;
public:
    // Класс итератора односвязного списка
    class iterator {
    public:
        iterator( node* val ) : m_node( val ) { }

        // Проверка на равенство
        bool operator==( const iterator& other ) const {
            if( this == &other ) {
                return true;
            }
            return m_node == other.m_node;
        }

        // Проверка на неравенство
        bool operator!=( const iterator& other ) const {
            return !operator==( other );
        }

        // Получение значения текущего узла
        T operator*() const {
            if( m_node ) {
                return m_node->m_t;
            } 
            return T();
        }

        // Переход к следующему узлу
        void operator++() {
            if( m_node ) {
                m_node = m_node->m_next;
            } 
        }

    private:
        node* m_node;
    };

public:
    my_list(): m_head(nullptr){}

    ~my_list(){
        while( m_head ) {
            remove();
        }
    };

    // Добавление узла в список
    void append( const T& t );

    // Удаление последнего добавленного узла из списка
    void remove();

    // Получить головной элемент списка
    T head() const;

    // Получить итератор на начало списка
    iterator begin() const;

    // Получить итератор на конец списка
    iterator end() const;

    // Получить размер списка
    size_t size() const;

    // Проверка на пустоту
    bool empty() const;

private:
    // Структура узла односвязного списка
    struct node {
        node() : m_next( NULL ) { }

        node( const T& t ) : m_t( t ), m_next( NULL ) { }

        // Значение узла
        T m_t;

        // Указатель на следующий узел
        node* m_next;
    };

    // Голова односвязного списка
    node* m_head;
};

template< typename T, typename A >
void my_list<T, A>::append( const T &t ) {
    // Создаем новый узел для значения
    if( node* m_node = traits_node::allocate(alloc,1) ) {
        traits_node::construct(alloc, m_node, t);
        // Новый узел привязывается к старому головному элементу
        m_node->m_next = m_head;
        // Новый узел сам становится головным элементом
        m_head = m_node;
    }
}

template< typename T, typename A  >
void my_list< T, A >::remove() {
    if( m_head ) { // Если список не пуст:
        // Сохраняем указатель на второй узел, который станет новым головным элементом
        node* new_head = m_head->m_next;

        // Освобождаем память, выделенную для удаляемого головного элемента
        traits_node::deallocate(alloc, m_head, 1);

        // Назначаем новый головной элемент
        m_head = new_head;
    }
}

template< typename T, typename A  >
typename my_list< T, A >::iterator my_list< T, A >::begin() const {
    return iterator( m_head );
}

template< typename T, typename A  >
typename my_list< T, A >::iterator my_list< T, A >::end() const {
    return iterator( nullptr );
}

template< typename T, typename A  >
size_t my_list< T, A >::size() const {
    size_t s = 0;
    for( iterator it = begin(); it != end(); ++it ) {
        ++s;
    }
    return s;
}

template< typename T, typename A  >
bool my_list< T, A >::empty() const {
    return m_head == nullptr;
}

