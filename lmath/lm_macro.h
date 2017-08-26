#ifndef lm_macro_h__
#define lm_macro_h__

#define EMPTY_SUFFIX
#define UNPACK(...) __VA_ARGS__
#define GEN_METHOD(_Method) _Method(EMPTY_SUFFIX) ENABLE_IF_AMP( _Method(restrict(amp)) )

#define GEN_METHOD_CONST(_Method) _Method(const) ENABLE_IF_AMP( _Method(const restrict(amp)) )

#define GEN_METHOD2(_Method) _Method(EMPTY_SUFFIX, EMPTY_SUFFIX) _Method(const, const) ENABLE_IF_AMP( _Method(EMPTY_SUFFIX, restrict(amp)) _Method(const, const restrict(amp)) )

#define GEN_METHOD_PARAMS(_Method, ...) UNPACK(_Method(__VA_ARGS__, EMPTY_SUFFIX)) UNPACK( ENABLE_IF_AMP( _Method(__VA_ARGS__, restrict(amp)) ))

#endif // lm_macro_h__
