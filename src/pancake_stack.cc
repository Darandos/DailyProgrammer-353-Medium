#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <map>

#ifndef PANCAKESTACK_H_
#define PANCAKESTACK_H_

#include <memory>
#include <vector>

namespace daily_programmer_20180307 {
	template <class SequenceContainer>
	class PancakeStack {
		int number_of_flips_ = 0;
		std::shared_ptr<std::vector<int>> pi_;
		std::unique_ptr<std::vector<int>> sorted_;
		
		void FlipAfter(std::vector<int>::iterator);
		std::vector<int>::iterator GetNext(const std::vector<int>::iterator) const;

		/**
		 * Wrap index around such that it always falls between 0 and size (exclusive)
		 * 
		 * Negative values are wrapped around to size
		 * 
		 * @param index The index to wrap
		 * @param size One more than the maximum value of index
		 * @returns A value that will always be within bounds of a collection of size size
		 */
		static int Wrap(int index, int size);

		/**
		 * Wrap index around such that it always falls within the range of pi
		 * 
		 * Negative values are wrapped around to the size of pi
		 * 
		 * @param index The index to wrap
		 * @returns A value that will always fall within the range of pi
		 */
		int Wrap(int index) const;
		
		public:
			PancakeStack(const SequenceContainer&);
			int get_number_of_flips() const;
			std::shared_ptr<const std::vector<int>> get_pi() const;
			friend std::ostream& operator<<(std::ostream& out, const PancakeStack<SequenceContainer>& stack) {
				for (auto i : *stack.pi_) {
					out << i << " ";
				}
				out << std::endl;
				return out;
			}
			int CountAdjacencies();
			bool EndsBlock(const int) const;
			void FlipAfter(const int);
			void FlipBefore(const int);
			int GetLastElementInBlock(const int) const;
			bool IsFree(const int) const;
			void Sort();
			bool StartsBlock(const int) const;
	};

	#endif // PANCAKESTACK_H_

	template <class SequenceContainer>
	PancakeStack<SequenceContainer>::PancakeStack(const SequenceContainer& sequence_container) {
		using std::vector;
		sorted_ = std::make_unique<vector<int>>(sequence_container.begin(), sequence_container.end());
		std::sort(sorted_->begin(), sorted_->end());
		
		pi_ = std::make_shared<vector<int>>(sequence_container.size());
		auto found_numbers = std::map<int, vector<int>::iterator>();
		for (int i = 0; i < sequence_container.size(); ++i) {
			int needle = sequence_container.at(i);
			vector<int>::iterator begin;
			if (found_numbers.count(needle) == 1) {
				begin = std::next(found_numbers.at(needle));
			} else {
				begin = sorted_->begin();
			}
			vector<int>::iterator found_number = std::find(begin, sorted_->end(), needle);
			pi_->at(i) = std::distance(sorted_->begin(), found_number);
			found_numbers[*found_number] = found_number;
		}
	}

	template <class SequenceContainer>
	int PancakeStack<SequenceContainer>::get_number_of_flips() const {
		return number_of_flips_;
	}

	template <class SequenceContainer>
	std::shared_ptr<const std::vector<int>> PancakeStack<SequenceContainer>::get_pi() const {
		return pi_;
	}

	template <class SequenceContainer>
	bool PancakeStack<SequenceContainer>::IsFree(const int t) const {
		using std::prev;
		
		auto t_iterator = std::find(pi_->begin(), pi_->end(), t);
		
		for (decltype(t_iterator) neighbor : {
			t_iterator == pi_->begin() ? prev(pi_->end()) : prev(t_iterator), // Element before t
			t_iterator == prev(pi_->end()) ? pi_->begin() : std::next(t_iterator) // Element after t
		}) {
			if (*neighbor == Wrap(*t_iterator + 1) || *neighbor == Wrap(*t_iterator - 1)) {
				return false;
			}
		}
		
		return true;
	}

	template <class SequenceContainer>
	void PancakeStack<SequenceContainer>::FlipAfter(const int t) {
		auto middle = std::find(pi_->begin(), pi_->end(), t);
		FlipAfter(middle);
	}

	template <class SequenceContainer>
	void PancakeStack<SequenceContainer>::FlipBefore(const int t) {
		auto middle = std::find(pi_->begin(), pi_->end(), t);
		std::advance(middle, -1);
		FlipAfter(middle);
	}

	template <class SequenceContainer>
	int PancakeStack<SequenceContainer>::Wrap(int index, int size) {
		// int result = index - index / size * size;
		// if (result < 0) result = size + result;
		int result = index % size;
		if (result < 0) result = size + result;
		return result;
	}

	template <class SequenceContainer>
	int PancakeStack<SequenceContainer>::Wrap(int index) const {
		return Wrap(index, pi_->size());
	}

	template <class SequenceContainer>
	void PancakeStack<SequenceContainer>::FlipAfter(std::vector<int>::iterator middle) {
		using std::advance;
		
		auto begin = pi_->begin();
		while (std::distance(begin, middle) > 0) {
			std::iter_swap(begin, middle);
			advance(begin, 1);
			advance(middle, -1);
		}

		number_of_flips_++;
	}

	template <class SequenceContainer>
	bool PancakeStack<SequenceContainer>::StartsBlock(const int t) const {
		using std::prev;
		
		auto t_iterator = std::find(pi_->begin(), pi_->end(), t);
		
		if (*t_iterator != t) { // t was not found
			throw std::out_of_range("t (" + std::to_string(t) + ") not found in pi");
		}
		
		auto before_t = std::distance(t_iterator, pi_->begin()) == 0 ? prev(pi_->end()) : prev(t_iterator);
		auto after_t = std::distance(t_iterator, prev(pi_->end())) == 0 ? pi_->begin() : std::next(t_iterator);
		
		return *before_t != Wrap(t + 1) && *before_t != Wrap(t - 1) && (*after_t == Wrap(t + 1) || *after_t == Wrap(t - 1));
	}

	template <class SequenceContainer>
	bool PancakeStack<SequenceContainer>::EndsBlock(const int t) const {
		using std::distance;
		using std::prev;
		
		auto t_iterator = std::find(pi_->begin(), pi_->end(), t);
		
		if (*t_iterator != t) {
			throw std::out_of_range("t (" + std::to_string(t) + ") not found in pi");
		}

		if (distance(t_iterator, pi_->end()) == 1) {
			return true;
		}
		
		auto before_t = distance(t_iterator, pi_->begin()) == 0 ? prev(pi_->end()) : prev(t_iterator);
		auto after_t = distance(t_iterator, prev(pi_->end())) == 0 ? pi_->begin() : std::next(t_iterator);
		
		return *after_t != Wrap(t + 1) && *after_t != Wrap(t - 1) && (*before_t == Wrap(t + 1) || *before_t == Wrap(t - 1));
	}

	template <class SequenceContainer>
	int PancakeStack<SequenceContainer>::GetLastElementInBlock(const int t) const {
		using std::prev;
		
		if (IsFree(t)) {
			throw new std::logic_error("t is not part of a block");
		}
		
		auto current = std::find(pi_->begin(), pi_->end(), t);
		
		if (*current != t) { // t was not found
			throw std::out_of_range("t (" + std::to_string(t) + ") not found in pi");
		}
		
		auto next = GetNext(current);
		while (!EndsBlock(*current)) {
			current = next;
			next = GetNext(current);
		}
		
		return *current;
	}

	template <class SequenceContainer>
	std::vector<int>::iterator PancakeStack<SequenceContainer>::GetNext(const std::vector<int>::iterator t_iterator) const {
		return std::distance(t_iterator, std::prev(pi_->end())) == 0 ? pi_->begin() : std::next(t_iterator);
	}

	template <class SequenceContainer>
	void PancakeStack<SequenceContainer>::Sort() {
		using std::distance;
		using std::find;
		using std::vector;

		while (CountAdjacencies() < pi_->size() - 1) {
			vector<int>::const_reference t = pi_->front();
			if (IsFree(t)) {
				if (IsFree(t + 1)) {
					FlipBefore(t + 1);
				} else if (IsFree(t -1)) {
					FlipBefore(t - 1);
				} else if (StartsBlock(t + 1)) {
					FlipBefore(t + 1);
				} else if (StartsBlock(t - 1)) {
					FlipBefore(t - 1);
				} else if (EndsBlock(t + 1) && EndsBlock(t - 1)) {
					// Perform flipping shown in figure c
					vector<int>::iterator t_iterator = pi_->begin();
					vector<int>::iterator t_plus_o_iterator = find(t_iterator, pi_->end(), Wrap(t + 1));
					vector<int>::iterator t_minus_o_iterator = find(t_iterator, pi_->end(), Wrap(t - 1));
					if (t_plus_o_iterator < t_minus_o_iterator) {
						std::swap(t_plus_o_iterator, t_minus_o_iterator);
					}
					int t_plus_o = *t_plus_o_iterator;
					int t_minus_o = *t_minus_o_iterator;

					FlipAfter(t_plus_o);
					FlipBefore(t);
					FlipAfter(t_minus_o);
					FlipBefore(t);
				} else {
					throw std::logic_error("This shouldn't happen");
				}
			} else { // t is in a block
				if (IsFree(Wrap(t + 1))) {
					FlipBefore(t + 1);
				} else if (IsFree(Wrap(t - 1))) {
					FlipBefore(t - 1);
				} else if (StartsBlock(Wrap(t + 1))) {
					FlipBefore(t + 1);
				} else if (StartsBlock(Wrap(t - 1))) {
					FlipBefore(t - 1);
				} else {
					int last_element_in_block = GetLastElementInBlock(t);
					int k = abs(t - last_element_in_block);
					int o = t < last_element_in_block ? 1 : -1;
					int t_minus_o = Wrap(t - o);
					assert(t_minus_o == GetLastElementInBlock(t_minus_o));
					int t_plus_k_plus_one_o = Wrap(t + (k + 1) * o);
					int t_plus_k_o = Wrap(t + k * o);

					if (IsFree(t_plus_k_plus_one_o)) {
						auto t_plus_k_plus_one_o_iterator = find(pi_->begin(), pi_->end(), t_plus_k_plus_one_o);
						auto t_minus_o_iterator = find(pi_->begin(), pi_->end(), t_minus_o);
						if (t_plus_k_plus_one_o_iterator > t_minus_o_iterator) {
							FlipAfter(t_plus_k_plus_one_o);
							FlipBefore(t_plus_k_o);
							FlipAfter(t_minus_o);
							FlipBefore(t);
						} else {
							FlipAfter(t_plus_k_plus_one_o);
							FlipBefore(t_plus_k_o);
							FlipAfter(t);
							FlipBefore(t_minus_o);
						}
					} else {
						if (EndsBlock(t_plus_k_plus_one_o)) {
							FlipAfter(t_plus_k_plus_one_o);
							FlipBefore(t_plus_k_o);
						} else {
							FlipAfter(t_plus_k_o);
							FlipBefore(t_plus_k_plus_one_o);
						}
					}
				}
			}
		}

		vector<int>::iterator zero_iterator = find(pi_->begin(), pi_->end(), 0);
		if (zero_iterator != pi_->begin()) {
			if (*std::next(zero_iterator) == pi_->size() - 1) {
				// Figure 1(d)
				vector<int>::const_reference k = pi_->front();
				FlipBefore(0);
				FlipAfter(k - 1);
				FlipAfter(0);
			} else {
				// Figure 1(c)
				vector<int>::const_reference k = pi_->back();
				FlipAfter(k);
				FlipBefore(0);
				FlipAfter(k - 1);
				FlipAfter(0);
			}
		}
	}

	template <class SequenceContainer>
	int PancakeStack<SequenceContainer>::CountAdjacencies() {
		int numberOfAdjacencies = 0;
		std::vector<int>::size_type size = pi_->size();
		for (int i = 0; i < size; i++) {
			if (abs(pi_->at(i) - pi_->at(Wrap(i + 1))) == 1) {
				numberOfAdjacencies++;
			}
		}
		return numberOfAdjacencies;
	}
}