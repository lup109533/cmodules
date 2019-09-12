#ifndef __CMODULES_MULTI_RETURN__
#define __CMODULES_MULTI_RETURN__

/** Macros to facilitate multiple-value returns from functions (up to 5 values). **/

// 2 values
#define multi_def(T1, T2) struct {T1 _1; T2 _2;}
#define multi_return(F, V1, V2) typeof(F) __ret; __ret._1 = V1; __ret._2 = V2; return __ret;
#define multi_call(V1, V2, F) {typeof(F) __call; __call = F; V1 = __call._1; V2 = __call._2;}

// values > 2
#define multi_def_(N) multi_def_ ## N
#define multi_return_(N) multi_return_ ## N
#define multi_call_(N) multi_call_ ## N

// 3 values
#define multi_def_3(T1, T2, T3) struct {T1 _1; T2 _2; T3 _3;}
#define multi_return_3(F, V1, V2, V3)   typeof(F) __ret;\
                                        __ret._1 = V1;\
                                        __ret._2 = V2;\
                                        __ret._3 = V3;\
                                        return __ret;
#define multi_call_3(V1, V2, V3, F) {\
                                        typeof(F) __call;\
                                        __call = F;\
                                        V1 = __call._1;\
                                        V2 = __call._2;\
                                        V3 = __call._3;\
                                    }

// 4 values
#define multi_def_4(T1, T2, T3, T4) struct {T1 _1; T2 _2; T3 _3; T4 _4;}
#define multi_return_4(F, V1, V2, V3, V4)   typeof(F) __ret;\
                                            __ret._1 = V1;\
                                            __ret._2 = V2;\
                                            __ret._3 = V3;\
                                            __ret._4 = V4;\
                                            return __ret;
#define multi_call_4(V1, V2, V3, V4, F) {\
                                            typeof(F) __call;\
                                            __call = F;\
                                            V1 = __call._1;\
                                            V2 = __call._2;\
                                            V3 = __call._3;\
                                            V4 = __call._4;\
                                        }

// 5 values
#define multi_def_5(T1, T2, T3, T4, T5) struct {T1 _1; T2 _2; T3 _3; T4 _4; T5 _5;}
#define multi_return_5(F, V1, V2, V3, V4, V5)   typeof(F) __ret;\
                                                __ret._1 = V1;\
                                                __ret._2 = V2;\
                                                __ret._3 = V3;\
                                                __ret._4 = V4;\
                                                return __ret;
#define multi_call_5(V1, V2, V3, V4, V5, F) {\
                                                typeof(F) __call;\
                                                __call = F;\
                                                V1 = __call._1;\
                                                V2 = __call._2;\
                                                V3 = __call._3;\
                                                V4 = __call._4;\
                                            }

#endif // __CMODULES_MULTI_RETURN__
