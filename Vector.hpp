#pragma once

#include <array>
#include <algorithm>
#include <string>
#include <functional>
#include <cmath>

#include "Utility.hpp"

namespace ta
{
	namespace swizzle
	{
		constexpr size_t x = 0;
		constexpr size_t y = 1;
		constexpr size_t z = 2;
		constexpr size_t w = 3;
	}

	template<class T, size_t Dim>
	class Vector
	{
		static_assert(Dim > 1, "Invalidate Dimention");
		//static_assert(std::is_arithmetic_v<T>, "Invaludate type");

	public:
		using value_type = T;
		using iterator = T*;
		using const_iterator = const T*;
		using this_type = Vector<T, Dim>;
		using value_ref_type = value_type&;

	private:
		value_type mData[Dim];

	public:
		std::pair<iterator, iterator> _get_range()
		{
			return { mData, mData + Dim };
		}
		std::pair<const_iterator, const_iterator> _get_range() const
		{
			return { mData, mData + Dim };
		}

		template<class Fn>
		Vector<T, Dim> transform_to_new(Fn&& _Func) const noexcept
		{
			Vector<T, Dim> result;
			auto [begin, end] = _get_range();
			auto [rbegin, _] = result._get_range();
			std::transform(begin, end, rbegin, _Func);
			return result;
		}

	private:

		template<size_t Idx0, size_t Idx1>
		Vector<T, 2> _get()
		{
			static_assert((Idx0 < Dim) && (Idx1 < Dim), "Invalidate swizzle arguments. Out of range");
			return Vector<T, 2>({ mData[Idx0], mData[Idx1] });
		};
		template<size_t Idx0, size_t Idx1, size_t Idx2>
		Vector<T, 3> _get()
		{
			static_assert((Idx0 < Dim) && (Idx1 < Dim) && (Idx2 < Dim), "Invalidate swizzle arguments. Out of range");
			return Vector<T, 3>{mData[Idx0], mData[Idx1], mData[Idx2]};
		};
		template<size_t Idx0, size_t Idx1, size_t Idx2, size_t Idx3>
		Vector<T, 4> _get()
		{
			static_assert((Idx0 < Dim) && (Idx1 < Dim) && (Idx2 < Dim) && (Idx3 < Dim), "Invalidate swizzle arguments. Out of range");
			return Vector<T, 4>{mData[Idx0], mData[Idx1], mData[Idx2], mData[Idx3] };
		}



	public:
		Vector(T _Val)
		{
			auto [begin, end] = _get_range();
			std::fill(begin, end, _Val);
		}
		Vector()
		{
			auto [begin, end] = _get_range();
			std::fill(begin, end, static_cast<T>(0));
		}

		Vector(const this_type&) = default;
		Vector(this_type&&) = default;

		Vector(std::initializer_list<T> _Init_List)
		{
			std::copy(_Init_List.begin(), _Init_List.end(), mData);
		}
		Vector(Vector<T, (Dim - 1)>& _Vec, T _Val)
		{
			auto [other_begin, other_end] = _Vec._get_range();
			std::copy(other_begin, other_end, mData);
			mData[Dim - 1] = _Val;
		}
		~Vector()
		{

		}

		static constexpr size_t size() noexcept
		{
			return Dim;
		}

		this_type& operator=(const Vector<T, Dim>&) = default;
		this_type& operator=(Vector<T, Dim>&&) = default;

		this_type operator-() const noexcept
		{
			return transform_to_new([](auto e) {return -e; });
		}

		this_type operator+() const noexcept
		{
			return *this;
		}

		value_ref_type operator[](size_t _Idx)
		{
			return mData[_Idx];
		}

		value_type operator[](size_t _Idx) const
		{
			return mData[_Idx];
		}

		template<class U>
		this_type& operator+=(const Vector<U, Dim>& _Right) noexcept
		{
			auto [right_begin, right_end] = _Right._get_range();
			auto [begin, end] = _get_range();
			std::transform(right_begin, right_end, begin, begin,
				[](auto a, auto b) {return static_cast<T>(a + b); });
			return *this;
		}

		template<class U>
		this_type& operator-=(const Vector<U, Dim>& _Right) noexcept
		{
			auto [right_begin, right_end] = _Right._get_range();
			auto [begin, end] = _get_range();
			std::transform(right_begin, right_end, begin, begin,
				[](auto a, auto b) {return static_cast<T>(b - a); });
			return *this;
		}

		// operator glm::vec3() const noexcept
		// {
		// 	static_assert(Dim >= 3, "Invalidate cast. ta::Vector is so small");
		// 	return glm::vec3{ mData[0],mData[1],mData[2] };
		// }

		// operator glm::vec4() const noexcept
		// {
		// 	static_assert(Dim >= 4, "Invalidate cast. ta::Vector is so small");
		// 	return glm::vec4{ mData[0],mData[1],mData[2],mData[3] };
		// }


		iterator begin() noexcept { return mData; }
		iterator end() noexcept { return mData + Dim; }
		const_iterator begin() const noexcept { return mData; }
		const_iterator end() const noexcept { return mData + Dim; }

		value_type* data() noexcept
		{
			return mData;
		}

		const value_type* data() const noexcept
		{
			return mData;
		}

		void* vdata() const noexcept
		{
			auto tmp = static_cast<const void*>(mData);
			return const_cast<void*>(tmp);
		}

		float length() const noexcept
		{
			return std::sqrt(static_cast<float>(dot2()));
		}

		value_type dot2() const noexcept
		{
			T res = static_cast<T>(0);
			for (auto& e : mData)
				res += e * e;
			return res;
		}

		std::string to_string() const
		{
			std::string result("{");
			bool not_first_value = false;
			for (auto& e : mData)
			{
				if (not_first_value)
					result += ", ";
				else
					not_first_value = true;

				if constexpr (std::is_arithmetic_v<T>)
					result += std::to_string(e);
				else
					result += std::to_string(static_cast<std::size_t>(e));
			}
			result += "}";
			return result;
		}

		template<class U = T>
		detail::enable_if_t<T, U&, (Dim > 0)> x()
		{
			return mData[0];
		}

		template<class U = T>
		detail::enable_if_t<T, U, (Dim > 0)> x() const
		{
			return mData[0];
		}

		template<class U = T>
		detail::enable_if_t<T, U&, (Dim > 1)> y()
		{
			return mData[1];
		}

		template<class U = T>
		detail::enable_if_t<T, U, (Dim > 1)> y() const
		{
			return mData[1];
		}

		template<class U = T>
		detail::enable_if_t<T, U&, (Dim > 2)> z()
		{
			return mData[2];
		}

		template<class U = T>
		detail::enable_if_t<T, U, (Dim > 2)> z() const
		{
			return mData[2];
		}

		template<class U = T>
		detail::enable_if_t<T, U&, (Dim > 3)> w()
		{
			return mData[3];
		}

		template<class U = T>
		detail::enable_if_t<T, U, (Dim > 3)> w() const
		{
			return mData[3];
		}

		template<size_t... I>
		auto swizzle()
		{
			if constexpr (sizeof...(I) >= 2 && sizeof...(I) <= 4)
				return _get<I...>();
			else
			{
				static_assert(sizeof...(I) >= 2 && sizeof...(I) <= 4, "Swizzle operator cano't ba called. Invalidate argument package size");
				return Vector<T, 0>();
			}
		}
	};



	template<class T, class U, size_t Dim>
	auto operator+(const Vector<T, Dim>& _V, const Vector<U, Dim>& _U) noexcept ->
		Vector<decltype(detail::declval_by_mul<T, U>()), Dim>
	{
		using return_type = decltype(detail::declval_by_mul<T, U>());

		Vector<return_type, Dim> result;
		auto [vbegin, vend] = _V._get_range();
		auto [ubegin, uend] = _U._get_range();
		auto [rbegin, rend] = result._get_range();

		std::transform(vbegin, vend, ubegin, rbegin,
			[](auto a, auto b) {return static_cast<return_type>(a + b); });
		return result;
	}

	template<class T, class U, size_t Dim>
	auto operator-(const Vector<T, Dim>& _V, const Vector<U, Dim>& _U) noexcept ->
		Vector<decltype(detail::declval_by_mul<T, U>()), Dim>
	{
		using return_type = decltype(detail::declval_by_mul<T, U>());

		Vector<return_type, Dim> result;
		auto [vbegin, vend] = _V._get_range();
		auto [ubegin, uend] = _U._get_range();
		auto [rbegin, rend] = result._get_range();

		std::transform(vbegin, vend, ubegin, rbegin,
			[](auto a, auto b) {return static_cast<return_type>(a - b); });
		return result;
	}

	Vector<float, 4> operator+(const Vector<float, 4>& _V, const Vector<float, 4>& _U) noexcept;
	Vector<float, 4> operator-(const Vector<float, 4>& _V, const Vector<float, 4>& _U) noexcept;

	template<class T, class U, size_t Dim>
	std::enable_if_t<std::is_arithmetic_v<U>, Vector<T, Dim>> operator*(const Vector<T, Dim>& _Vec, U _Val) noexcept
	{
		return _Vec.transform_to_new([_Val](auto e) {return static_cast<T>(_Val * e); });
	}

	template<class T, class U, size_t Dim>
	std::enable_if_t<std::is_arithmetic_v<U>, Vector<T, Dim>> operator/(const Vector<T, Dim>& _Vec, U _Val) noexcept
	{
		return _Vec.transform_to_new([_Val](auto e) {return static_cast<T>(e / _Val); });
	}

	template<class T, class U, size_t Dim>
	std::enable_if_t<std::is_arithmetic_v<U>, Vector<T, Dim>> operator*(U _Val, const Vector<T, Dim>& _Vec) noexcept
	{
		return _Vec.transform_to_new([_Val](auto e) {return static_cast<T>(_Val * e); });
	}

	template<class T, class U, size_t Dim>
	std::enable_if_t<std::is_arithmetic_v<U>, Vector<T, Dim>> operator/(U _Val, const Vector<T, Dim>& _Vec) noexcept
	{
		return _Vec.transform_to_new([_Val](auto e) {return static_cast<T>(_Val / e); });
	}
};