#ifndef MY_TYPE_TRAITS_H
#define MY_TYPE_TRAITS_H
#pragma once

namespace my {

	/**
	 * @brief 主模版，用于判断是否是显式接受一个类型参数的模版
	 * @tparam T 	待判断的类型/模版实例
	 */
	template <typename T>
	struct is_single_tparam_template : std::false_type {};

	/**
	 * @brief 对显式地只包含一个类型参数的模版进行类型萃取，并且能够替换该类型参数
	 * @tparam Template 显式只接受一个类型参数的模版，通过 ::substitution<Param> 进行替换
	 * @tparam Tparam	类型参数，被萃取为 ::type
	 */
	template <template <class...> typename Template, typename Tparam>
	struct is_single_tparam_template<Template<Tparam>> : std::true_type {
		using type = Tparam;
		template <typename Param>
		using substitution = Template<Param>;
	};

	template <typename T>
	constexpr auto is_single_tparam_template_v = is_single_tparam_template<T>::value;

	template <typename T>
	using is_single_tparam_template_t = typename is_single_tparam_template<T>::type;

	template <typename T, typename Rep>
	using replace_only_tparam_t = typename is_single_tparam_template<T>::template substitution<Rep>;

}

#endif //MY_TYPE_TRAITS_H
