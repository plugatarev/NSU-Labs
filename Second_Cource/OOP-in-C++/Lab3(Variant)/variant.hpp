#pragma once
#include <iostream>
#include "Exception.hpp"
#include "Is_Same.hpp"

namespace UtilsVariant{
    template<typename... Args>
    struct is_type;

    template<typename T, typename F, typename... Args>
    struct is_type<T, F, Args...> {
        static constexpr bool is(){
            if (My::is_same<T,F>::value) return true;
            else{
                if (is_type<T, Args...>::is()) return true;
            }
            return false;
        }
    };

    template<typename T> struct is_type<T>{
        constexpr static bool is() {
            return false;
        }
    };
}

namespace MyVariant{
    //at least one arg
    template<typename F, typename... Args>
    class variant{
    public: 
        //Default constructor. data assign nullptr
        variant(){}

        //Makes a copy of value, so that the initial content of the new instance is equivalent in both type and value to value.
        template<typename T> variant(const T& value):data(new impl<T>(value)){
            static_assert(UtilsVariant::is_type<T, F, Args...>::is());
        }

        //Makes a copy of value, discarding previous content, so that the new content of is equivalent in both type and value to value.
        template<typename T>
        variant& operator=(const T& value){
            static_assert(UtilsVariant::is_type<T, F, Args...>::is());
            delete data;
            data = new impl<T>(value);
            return *this;
        }

        //Copy constructor. Сannot give variant with different stored types(F, Args...).
        variant(const variant<F,Args...>& old):data(copy<F, Args...>::copy_val(old.data)){}

        variant& operator=(const variant<F,Args...>& old){
            delete data;
            data = copy<F, Args...>::copy_val(old.data);
            return *this;
        }

        //Move constructor. Сannot give variant with different stored types(F, Args...).
        variant(variant<F,Args...>&& old){
            std::swap(data, old.data);
        }

        // // Serves as both the move and the copy asignment operator.
        variant<F,Args...>& operator=(variant<F,Args...>&& old){ // - ok
            std::swap(data, old.data);
            return *this;
        }

        //Returns the value of the variant given the type, which can stored in this variant.
        //If the variant stores not the type T then throw variant_access_error exception with message(This type isn't stored in this variant).
        template<typename T>
        T get(){
            static_assert(UtilsVariant::is_type<T, F, Args...>::is());
            return (dynamic_cast<impl<T>*>(data) != nullptr ? dynamic_cast<impl<T>*>(data)->val : throw variant_access_error("This type isn't stored in this variant"));
        }
        
        //Returns the point of value of the variant given the type, which can stored in this variant.
        //If the variant stores not the type T then returns nullptr.
        template<typename T>
        T * get_if(){
            static_assert(UtilsVariant::is_type<T, F, Args...>::is());
            if (UtilsVariant::is_type<T, Args...>::is())
                return &(dynamic_cast<impl<T>*>(data)->val);
            else 
                return nullptr;
        }
        
        //Destructor
        ~variant() {
            if (data != nullptr) delete data;
        }

    private: 
        struct placeholder{
            virtual ~placeholder() {}
        };

        template < typename T >
        struct impl: placeholder{
            impl(){};
            impl(T const& t):val(t){}
            ~impl(){}
            T val;

        };

        template<typename... Ts>
        struct copy;

        // template<typename G, typename... Ts>
        // struct copy<G, Ts...> {
        //     static placeholder* copy_val(placeholder* old) {
        //         if (dynamic_cast<impl<G>*>(old)){
        //             return new impl<G>(*dynamic_cast<impl<G>*>(old));
        //         }
        //         else return copy<Ts...>::copy_val(old);
        //     }   
        // };
        template<typename G, typename... Ts>
        struct copy<G, Ts...> {
           static placeholder* copy_val(placeholder* old) {
               auto * impl_cast = dynamic_cast<impl<G> *>(old);
               if (impl_cast != nullptr) return new impl<G>(impl_cast->val);
               return copy<Ts...>::copy_val(old);
           }
       };

        template<typename G>
        struct copy<G> {
            static placeholder* copy_val(placeholder* old) {
                auto * impl_cast = dynamic_cast<impl<G> *>(old);
                if (impl_cast != nullptr) return new impl<G>(impl_cast->val);
                return nullptr;  
            } 
        };

       

//         template<typename G>
//         struct copy<G> {
//             static placeholder* copy_val(placeholder* old) {
//                 //                auto * impl_cast = dynamic_cast<impl<G> *>(old);
// //                if (impl_cast != nullptr) return new impl<G>(impl_cast->val);
//                 if (dynamic_cast<impl<G>*>(old)){
//                     return new impl<G>(*dynamic_cast<impl<G>*>(old));
//                 }
//                 return nullptr;
//             }   
//         };



        placeholder* data{nullptr};
    };
}


//VERSION with shared_ptr
// namespace UtilsVariant{
//     template<typename... Args>
//     struct is_type;

//     template<typename T, typename F, typename... Args>
//     struct is_type<T, F, Args...> {
//         static constexpr bool is(){
//             if (My::is_same<T,F>::value) return true;
//             else{
//                 if (is_type<T, Args...>::is()) return true;
//             }
//             return false;
//         }
//     };

//     template<typename T> struct is_type<T>{
//         constexpr static bool is() {
//             return false;
//         }
//     };

// }

// namespace MyVariant{
//     // At least one arg
//     template<typename F, typename... Args>
//     class variant{
//     public: 
//         //Default constructor. data assign nullptr
//         variant():data(nullptr){}

//         //Makes a copy of value, so that the initial content of the new instance is equivalent in both type and value to value.
//         template<typename T> variant(const T& value):data(new impl(value)){
//             static_assert(UtilsVariant::is_type<T, F, Args...>::is());
//         }

//         //Makes a copy of value, discarding previous content, so that the new content of is equivalent in both type and value to value.
//         template<typename T>
//         variant& operator=(const T& value){
//             static_assert(UtilsVariant::is_type<T, F, Args...>::is());
//             data = std::make_shared<impl<T>>(value);
//             return *this;
//         }

//         //Copy constructor. Сannot give variant with different stored types(F, Args...).
//         variant(const variant<F,Args...>& old):data(old.data){}

//         //Move constructor. Сannot give variant with different stored types(F, Args...).
//         variant(variant<F,Args...>&& old){
//             std::swap(data, old.data);
//         }

//         //Serves as both the move and the copy asignment operator.
//         variant<F,Args...>& operator=(variant<F,Args...>&& old){
//             std::swap(data, old.data);
//             return *this;
//         }
//         //Returns the value of the variant given the type, which can stored in this variant.
//         //If the variant stores not the type T then throw variant_access_error exception with message(This type isn't stored in this variant).
//         template<typename T>
//         T get(){
//             static_assert(UtilsVariant::is_type<T, F, Args...>::is());
//             return (dynamic_cast<impl<T>*>(data.get()) != nullptr ? dynamic_cast<impl<T>*>(data.get())->val : throw variant_access_error("This type isn't stored in this variant"));
//         }
        
//         //Returns the point of value of the variant given the type, which can stored in this variant.
//         //If the variant stores not the type T then returns nullptr.
//         template<typename T>
//         T * get_if(){
//             static_assert(UtilsVariant::is_type<T, F, Args...>::is());
//             if (UtilsVariant::is_type<T, Args...>::is())
//                 return dynamic_cast<impl<T>*>(data);
//             else 
//                 return nullptr;
//         }

//         //Destructor variant
//         ~variant(){}

//     private: 
//         struct placeholder{
//             virtual ~placeholder() {}
//         };

//         template < typename T >
//         struct impl: placeholder{
//             impl(){};
//             impl(T const& t):val(t){}
//             ~impl(){}
//             T val;

//         };
//         std::shared_ptr<placeholder> data;
//     };
// }