#pragma once

#include "priority_queue_list.h"
#include "../list/array_list.h"
#include <exception>

namespace structures
{
	/// <summary> Prioritny front implementovany haldou. Implementujucim typom haldy je struktura ArrayList. </summary>
	/// <typeparam name = "T"> Typ dat ukladanych v prioritnom fronte. </typepram>
	template<typename T>
	class Heap : public PriorityQueueList<T>
	{
	public:
		/// <summary> Konstruktor. </summary>
		Heap();

		/// <summary> Kopirovaci konstruktor. </summary>
		/// <param name = "other"> Halda, z ktorej sa prevezmu vlastnosti. </param>
		Heap(const Heap<T>& other);

		/// <summary> Operacia klonovania. Vytvori a vrati duplikat haldy. </summary>
		/// <returns> Ukazovatel na klon struktury. </returns>
		Structure* clone() const override;

		/// <summary> Operator priradenia. </summary>
		/// <param name = "other"> Prioritny front implementovany zoznamom, z ktoreho ma prebrat vlastnosti. </param>
		/// <returns> Adresa, na ktorej sa tento prioritny front nachadza po priradeni. </returns>
		/// <exception cref="std::logic_error"> Vyhodena, ak vstupny parameter nie je halda. </exception>  
		/// <remarks> Vyuziva typovy operator priradenia. </remarks>
		virtual PriorityQueueList<T>& operator=(const PriorityQueueList<T>& other) override;

		/// <summary> Operator priradenia pre haldu. </summary>
		/// <param name = "other"> Halda, z ktorej ma prebrat vlastnosti. </param>
		/// <returns> Adresa, na ktorej sa tato halda nachadza po priradeni. </returns>
		virtual Heap<T>& operator=(const Heap<T>& other);

		/// <summary> Vlozi prvok s danou prioritou do haldy. </summary>
		/// <param name = "priority"> Priorita vkladaneho prvku. </param>
		/// <param name = "data"> Vkladany prvok. </param>
		void push(const int priority, const T& data) override;

		/// <summary> Odstrani prvok s najvacsou prioritou z haldy. </summary>
		/// <returns> Odstraneny prvok. </returns>
		/// <exception cref="std::logic_error"> Vyhodena, ak je halda prazdna. </exception>  
		T pop() override;

	protected:
		/// <summary> Vrati index v ArrayList-e, na ktorom sa nachadza prvok s najvacsou prioritou. </summary>
		/// <returns> Index prvku s najvacsou prioritou. </returns>
		/// <exception cref="std::logic_error"> Vyhodena, ak je ArrayList prazdny. </exception>  
		int indexOfPeek() const override;

	private:
		/// <summary> Vrati index v ArrayList-e, na ktorom by sa mal nachadzat otec daneho prvku. </summary>
		/// <param name = "index"> Index prvku, ktoreho otca chceme vratit. </param>
		/// <returns> Index, na ktorom ba sa mal nachadzat otec prvku. </returns>
		int getParentIndex(const int index);

		/// <summary> Vrati index v ArrayList-e, na ktorom by mal mat dany prvok syna s najvacsou prioritou. </summary>
		/// <param name = "index"> Index prvku, ktoreho syna chceme vratit. </param>
		/// <returns> Index, na ktorom by sa mal nachadzat syn prvku. </returns>
		int getGreaterSonIndex(const int index);
	};

	template<typename T>
	Heap<T>::Heap() :
		PriorityQueueList<T>(new ArrayList<PriorityQueueItem<T>*>())
	{
	}

	template<typename T>
	Heap<T>::Heap(const Heap<T>& other) :
		Heap<T>()
	{
		*this = other;
	}

	template<typename T>
	Structure* Heap<T>::clone() const
	{
		return new Heap<T>(*this);
	}

	template<typename T>
	inline PriorityQueueList<T>& Heap<T>::operator=(const PriorityQueueList<T>& other)
	{
		return *this = dynamic_cast<const Heap<T>&>(other);
	}

	template<typename T>
	inline Heap<T>& Heap<T>::operator=(const Heap<T>& other)
	{
		if (this != &other)
		{
			PriorityQueueList<T>::operator=(other);
		}
		return *this;
	}

	template<typename T>
	void Heap<T>::push(const int priority, const T& data)
	{
		list_->add(new PriorityQueueItem<T>(priority, data));
		int aktualny = size() - 1;
		int otec = getParentIndex(aktualny);

		while (otec != -1 && (*list_)[otec]->getPriority() > (*list_)[aktualny]->getPriority()) {
			DSRoutines::swap<PriorityQueueItem<T>*>((*list_)[aktualny], (*list_)[otec]);
			aktualny = otec;
			otec = getParentIndex(aktualny);
		}
	}

	template<typename T>
	T Heap<T>::pop()
	{
		T data = (*list_)[0]->accessData();
		delete (*list_)[0];
		if (list_->size() > 1)
		{
			(*list_)[0] = list_->removeAt(static_cast<int>(list_->size() - 1));
		}
		else
		{
			list_->removeAt(static_cast<int>(list_->size() - 1));
		}

		int aktualny = 0;
		int syn = getGreaterSonIndex(aktualny);

		while (syn < list_->size() && (*list_)[aktualny]->getPriority() >(*list_)[syn]->getPriority())
		{
			DSRoutines::swap<PriorityQueueItem<T>*>((*list_)[aktualny], (*list_)[syn]);
			aktualny = syn;
			syn = getGreaterSonIndex(aktualny);
		}

		return data;
	}

	template<typename T>
	inline int Heap<T>::getParentIndex(const int index)
	{

		if (index <= 0)
			return -1;

		return (index + 1) / 2 - 1;

	}

	template<typename T>
	inline int Heap<T>::getGreaterSonIndex(const int index)
	{

		int pravy(index * 2 + 2);
		int lavy(index * 2 + 1);

		if (lavy >= size()) {

			return -1;

		}
		if (pravy >= size()) {

			return lavy;

		}

		return (*list_)[lavy]->getPriority() < (*list_)[pravy]->getPriority() ? lavy : pravy;

	}

	template<typename T>
	inline int Heap<T>::indexOfPeek() const
	{

		return size() == 0 ? -1 : 0;

	}
}