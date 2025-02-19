#ifndef INTERNAL_VARIABLE_H                              /*-*-C-*-vi:se ft=c:*/
#define INTERNAL_VARIABLE_H
/**
 * @author     Ruby developers <ruby-core@ruby-lang.org>
 * @copyright  This  file  is   a  part  of  the   programming  language  Ruby.
 *             Permission  is hereby  granted,  to  either redistribute  and/or
 *             modify this file, provided that  the conditions mentioned in the
 *             file COPYING are met.  Consult the file for details.
 * @brief      Internal header for variables.
 */
#include "ruby/internal/config.h"
#include <stddef.h>             /* for size_t */
#include "constant.h"           /* for rb_const_entry_t */
#include "ruby/internal/stdbool.h"     /* for bool */
#include "ruby/ruby.h"          /* for VALUE */
#include "shape.h"              /* for rb_shape_t */

/* global variable */

#define ROBJECT_TRANSIENT_FLAG    FL_USER2

/* variable.c */
void rb_gc_mark_global_tbl(void);
void rb_gc_update_global_tbl(void);
size_t rb_generic_ivar_memsize(VALUE);
VALUE rb_search_class_path(VALUE);
VALUE rb_attr_delete(VALUE, ID);
void rb_autoload_str(VALUE mod, ID id, VALUE file);
VALUE rb_autoload_at_p(VALUE, ID, int);
NORETURN(VALUE rb_mod_const_missing(VALUE,VALUE));
rb_gvar_getter_t *rb_gvar_getter_function_of(ID);
rb_gvar_setter_t *rb_gvar_setter_function_of(ID);
void rb_gvar_readonly_setter(VALUE v, ID id, VALUE *_);
void rb_gvar_ractor_local(const char *name);
static inline bool ROBJ_TRANSIENT_P(VALUE obj);
static inline void ROBJ_TRANSIENT_SET(VALUE obj);
static inline void ROBJ_TRANSIENT_UNSET(VALUE obj);

/**
 * Sets the name of a module.
 *
 * Non-permanently named classes can have a temporary name assigned (or
 * cleared). In that case the name will be used for `#inspect` and `#to_s`, and
 * nested classes/modules will be named with the temporary name as a prefix.
 *
 * After the module is assigned to a constant, the temporary name will be
 * discarded, and the name will be computed based on the nesting.
 *
 * @param[in]  mod        An instance of ::rb_cModule.
 * @param[in]  name       An instance of ::rb_cString.
 * @retval     mod
 */
VALUE rb_mod_set_temporary_name(VALUE, VALUE);

struct gen_ivtbl;
int rb_gen_ivtbl_get(VALUE obj, ID id, struct gen_ivtbl **ivtbl);
int rb_obj_evacuate_ivs_to_hash_table(ID key, VALUE val, st_data_t arg);

RUBY_SYMBOL_EXPORT_BEGIN
/* variable.c (export) */
void rb_mark_and_update_generic_ivar(VALUE);
void rb_mv_generic_ivar(VALUE src, VALUE dst);
VALUE rb_const_missing(VALUE klass, VALUE name);
int rb_class_ivar_set(VALUE klass, ID vid, VALUE value);
void rb_iv_tbl_copy(VALUE dst, VALUE src);
RUBY_SYMBOL_EXPORT_END

VALUE rb_ivar_lookup(VALUE obj, ID id, VALUE undef);
VALUE rb_gvar_get(ID);
VALUE rb_gvar_set(ID, VALUE);
VALUE rb_gvar_defined(ID);
void rb_const_warn_if_deprecated(const rb_const_entry_t *, VALUE, ID);
rb_shape_t * rb_grow_iv_list(VALUE obj);
void rb_ensure_iv_list_size(VALUE obj, uint32_t len, uint32_t newsize);
struct gen_ivtbl *rb_ensure_generic_iv_list_size(VALUE obj, rb_shape_t *shape, uint32_t newsize);
attr_index_t rb_obj_ivar_set(VALUE obj, ID id, VALUE val);

static inline bool
ROBJ_TRANSIENT_P(VALUE obj)
{
#if USE_TRANSIENT_HEAP
    return FL_TEST_RAW(obj, ROBJECT_TRANSIENT_FLAG);
#else
    return false;
#endif
}

static inline void
ROBJ_TRANSIENT_SET(VALUE obj)
{
#if USE_TRANSIENT_HEAP
    FL_SET_RAW(obj, ROBJECT_TRANSIENT_FLAG);
#endif
}

static inline void
ROBJ_TRANSIENT_UNSET(VALUE obj)
{
#if USE_TRANSIENT_HEAP
    FL_UNSET_RAW(obj, ROBJECT_TRANSIENT_FLAG);
#endif
}

#endif /* INTERNAL_VARIABLE_H */
