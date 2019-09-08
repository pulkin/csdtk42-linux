#line 1 "ext/hpricot_scan/hpricot_scan.rl"
/*
 * hpricot_scan.rl
 *
 * $Author: why $
 * $Date: 2006-05-08 22:03:50 -0600 (Mon, 08 May 2006) $
 *
 * Copyright (C) 2006 why the lucky stiff
 */
#include <ruby.h>

static VALUE sym_xmldecl, sym_doctype, sym_procins, sym_stag, sym_etag, sym_emptytag, sym_comment,
      sym_cdata, sym_text;
static ID s_read, s_to_str;

#define ELE(N) \
  if (tokend > tokstart) { \
    ele_open = 0; \
    rb_yield_tokens(sym_##N, tag, attr, tokstart == 0 ? Qnil : rb_str_new(tokstart, tokend-tokstart), taint); \
  }

#define SET(N, E) \
  if (mark_##N == NULL || E == mark_##N) \
    N = rb_str_new2(""); \
  else if (E > mark_##N) \
    N = rb_str_new(mark_##N, E - mark_##N);

#define CAT(N, E) if (NIL_P(N)) { SET(N, E); } else { rb_str_cat(N, mark_##N, E - mark_##N); }

#define SLIDE(N) if ( mark_##N > tokstart ) mark_##N = buf + (mark_##N - tokstart);

#define ATTR(K, V) \
    if (!NIL_P(K)) { \
      if (NIL_P(attr)) attr = rb_hash_new(); \
      rb_hash_aset(attr, K, V); \
    }

#line 152 "ext/hpricot_scan/hpricot_scan.rl"



#line 43 "ext/hpricot_scan/hpricot_scan.c"
static int hpricot_scan_start = 213;

static int hpricot_scan_error = 205;

#line 155 "ext/hpricot_scan/hpricot_scan.rl"

#define BUFSIZE 16384

void rb_yield_tokens(VALUE sym, VALUE tag, VALUE attr, VALUE raw, int taint)
{
  VALUE ary;
  if (sym == sym_text) {
    raw = tag;
  }
  ary = rb_ary_new3(4, sym, tag, attr, raw);
  if (taint) { 
    OBJ_TAINT(ary);
    OBJ_TAINT(tag);
    OBJ_TAINT(attr);
    OBJ_TAINT(raw);
  }
  rb_yield(ary);
}

VALUE hpricot_scan(VALUE self, VALUE port)
{
  static char buf[BUFSIZE];
  int cs, act, have = 0, nread = 0, curline = 1, text = 0;
  char *tokstart = 0, *tokend = 0;

  VALUE attr = Qnil, tag = Qnil, akey = Qnil, aval = Qnil;
  char *mark_tag = 0, *mark_akey = 0, *mark_aval = 0;
  int done = 0, ele_open = 0;

  int taint = OBJ_TAINTED( port );
  if ( !rb_respond_to( port, s_read ) )
  {
    if ( rb_respond_to( port, s_to_str ) )
    {
      port = rb_funcall( port, s_to_str, 0 );
      StringValue(port);
    }
    else
    {
      rb_raise( rb_eArgError, "bad argument, String or IO only please." );
    }
  }

  
#line 93 "ext/hpricot_scan/hpricot_scan.c"
	{
	cs = hpricot_scan_start;
	tokstart = 0;
	tokend = 0;
	act = 0;
	}
#line 199 "ext/hpricot_scan/hpricot_scan.rl"
  
  while ( !done ) {
    VALUE str;
    char *p = buf + have, *pe;
    int len, space = BUFSIZE - have;

    if ( space == 0 ) {
      /* We've used up the entire buffer storing an already-parsed token
       * prefix that must be preserved. */
      fprintf(stderr, "OUT OF BUFFER SPACE\n" );
      exit(1);
    }

    if ( rb_respond_to( port, s_read ) )
    {
      str = rb_funcall( port, s_read, 1, INT2FIX(space) );
    }
    else
    {
      str = rb_str_substr( port, nread, space );
    }

    StringValue(str);
    memcpy( p, RSTRING(str)->ptr, RSTRING(str)->len );
    len = RSTRING(str)->len;
    nread += len;

    /* If this is the last buffer, tack on an EOF. */
    if ( len < space ) {
      p[len++] = 0;
      done = 1;
    }

    pe = p + len;
    
#line 136 "ext/hpricot_scan/hpricot_scan.c"
	{
	p -= 1;
	if ( ++p == pe )
		goto _out;
	switch ( cs )
	{
tr11:
#line 128 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p;{ {p--;{goto st210;}} }p--;}
	goto st213;
tr15:
#line 135 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p;{
      if (text == 0)
      {
        if (ele_open == 1) {
          ele_open = 0;
          if (tokstart > 0) {
            mark_tag = tokstart;
          }
        } else {
          mark_tag = p;
        }
        attr = Qnil;
        tag = Qnil;
        text = 1;
      }
    }p--;}
	goto st213;
tr20:
#line 135 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{
      if (text == 0)
      {
        if (ele_open == 1) {
          ele_open = 0;
          if (tokstart > 0) {
            mark_tag = tokstart;
          }
        } else {
          mark_tag = p;
        }
        attr = Qnil;
        tag = Qnil;
        text = 1;
      }
    }}
	goto st213;
tr21:
#line 80 "ext/hpricot_scan/hpricot_scan.rl"
	{curline += 1;}
#line 135 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{
      if (text == 0)
      {
        if (ele_open == 1) {
          ele_open = 0;
          if (tokstart > 0) {
            mark_tag = tokstart;
          }
        } else {
          mark_tag = p;
        }
        attr = Qnil;
        tag = Qnil;
        text = 1;
      }
    }}
	goto st213;
tr64:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{	switch( act ) {
	case 2:
	{ ELE(doctype); }
	break;
	case 4:
	{ ELE(stag); }
	break;
	case 6:
	{ ELE(emptytag); }
	break;
	case 9:
	{
      if (text == 0)
      {
        if (ele_open == 1) {
          ele_open = 0;
          if (tokstart > 0) {
            mark_tag = tokstart;
          }
        } else {
          mark_tag = p;
        }
        attr = Qnil;
        tag = Qnil;
        text = 1;
      }
    }
	break;
	default: break;
	}
	{p = ((tokend))-1;}}
	goto st213;
tr65:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ ELE(stag); }}
	goto st213;
tr72:
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ ELE(stag); }}
	goto st213;
tr123:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ ELE(stag); }}
	goto st213;
tr165:
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ ELE(stag); }}
	goto st213;
tr262:
#line 128 "ext/hpricot_scan/hpricot_scan.rl"
	{{ {{p = ((tokend))-1;}{goto st210;}} }{p = ((tokend))-1;}}
	goto st213;
tr293:
#line 126 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ ELE(xmldecl); }}
	goto st213;
tr294:
#line 135 "ext/hpricot_scan/hpricot_scan.rl"
	{{
      if (text == 0)
      {
        if (ele_open == 1) {
          ele_open = 0;
          if (tokstart > 0) {
            mark_tag = tokstart;
          }
        } else {
          mark_tag = p;
        }
        attr = Qnil;
        tag = Qnil;
        text = 1;
      }
    }{p = ((tokend))-1;}}
	goto st213;
tr300:
#line 127 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ ELE(doctype); }}
	goto st213;
tr312:
#line 130 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ ELE(etag); }}
	goto st213;
tr317:
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ ELE(stag); }}
	goto st213;
tr326:
#line 55 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(tag, p); }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ ELE(stag); }}
	goto st213;
tr329:
#line 55 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(tag, p); }
#line 127 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ ELE(doctype); }}
	goto st213;
tr333:
#line 55 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(tag, p); }
#line 130 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ ELE(etag); }}
	goto st213;
tr354:
#line 133 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ {goto st206;} }}
	goto st213;
tr355:
#line 131 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ ELE(emptytag); }}
	goto st213;
tr356:
#line 132 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ {goto st201;} }}
	goto st213;
tr367:
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;{ ELE(stag); }}
	goto st213;
st213:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokstart = 0;}
	if ( ++p == pe )
		goto _out213;
case 213:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokstart = p;}
#line 367 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 10: goto tr21;
		case 60: goto tr22;
	}
	goto tr20;
tr22:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 40 "ext/hpricot_scan/hpricot_scan.rl"
	{
    if (text == 1) {
      CAT(tag, p);
      ELE(text);
      text = 0;
    }
    attr = Qnil;
    tag = Qnil;
    mark_tag = NULL;
    ele_open = 1;
  }
#line 135 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 9;}
	goto st214;
st214:
	if ( ++p == pe )
		goto _out214;
case 214:
#line 395 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 33: goto st0;
		case 47: goto st59;
		case 58: goto tr18;
		case 63: goto st144;
		case 95: goto tr18;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr18;
	} else if ( (*p) >= 65 )
		goto tr18;
	goto tr15;
st0:
	if ( ++p == pe )
		goto _out0;
case 0:
	switch( (*p) ) {
		case 45: goto st1;
		case 68: goto st2;
		case 91: goto st53;
	}
	goto tr294;
st1:
	if ( ++p == pe )
		goto _out1;
case 1:
	if ( (*p) == 45 )
		goto tr356;
	goto tr294;
st2:
	if ( ++p == pe )
		goto _out2;
case 2:
	if ( (*p) == 79 )
		goto st3;
	goto tr294;
st3:
	if ( ++p == pe )
		goto _out3;
case 3:
	if ( (*p) == 67 )
		goto st4;
	goto tr294;
st4:
	if ( ++p == pe )
		goto _out4;
case 4:
	if ( (*p) == 84 )
		goto st5;
	goto tr294;
st5:
	if ( ++p == pe )
		goto _out5;
case 5:
	if ( (*p) == 89 )
		goto st6;
	goto tr294;
st6:
	if ( ++p == pe )
		goto _out6;
case 6:
	if ( (*p) == 80 )
		goto st7;
	goto tr294;
st7:
	if ( ++p == pe )
		goto _out7;
case 7:
	if ( (*p) == 69 )
		goto st8;
	goto tr294;
st8:
	if ( ++p == pe )
		goto _out8;
case 8:
	if ( (*p) == 32 )
		goto st9;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st9;
	goto tr294;
st9:
	if ( ++p == pe )
		goto _out9;
case 9:
	switch( (*p) ) {
		case 32: goto st9;
		case 58: goto tr307;
		case 95: goto tr307;
	}
	if ( (*p) < 65 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st9;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr307;
	} else
		goto tr307;
	goto tr294;
tr307:
#line 52 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_tag = p; }
	goto st10;
st10:
	if ( ++p == pe )
		goto _out10;
case 10:
#line 503 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr327;
		case 62: goto tr329;
		case 91: goto tr330;
		case 95: goto st10;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st10;
		} else if ( (*p) >= 9 )
			goto tr327;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st10;
		} else if ( (*p) >= 65 )
			goto st10;
	} else
		goto st10;
	goto tr294;
tr327:
#line 55 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(tag, p); }
	goto st11;
st11:
	if ( ++p == pe )
		goto _out11;
case 11:
#line 533 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st11;
		case 62: goto tr300;
		case 80: goto st12;
		case 83: goto st48;
		case 91: goto st26;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st11;
	goto tr294;
st12:
	if ( ++p == pe )
		goto _out12;
case 12:
	if ( (*p) == 85 )
		goto st13;
	goto tr294;
st13:
	if ( ++p == pe )
		goto _out13;
case 13:
	if ( (*p) == 66 )
		goto st14;
	goto tr294;
st14:
	if ( ++p == pe )
		goto _out14;
case 14:
	if ( (*p) == 76 )
		goto st15;
	goto tr294;
st15:
	if ( ++p == pe )
		goto _out15;
case 15:
	if ( (*p) == 73 )
		goto st16;
	goto tr294;
st16:
	if ( ++p == pe )
		goto _out16;
case 16:
	if ( (*p) == 67 )
		goto st17;
	goto tr294;
st17:
	if ( ++p == pe )
		goto _out17;
case 17:
	if ( (*p) == 32 )
		goto st18;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st18;
	goto tr294;
st18:
	if ( ++p == pe )
		goto _out18;
case 18:
	switch( (*p) ) {
		case 32: goto st18;
		case 34: goto st19;
		case 39: goto st30;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st18;
	goto tr294;
st19:
	if ( ++p == pe )
		goto _out19;
case 19:
	switch( (*p) ) {
		case 9: goto tr320;
		case 34: goto tr319;
		case 61: goto tr320;
		case 95: goto tr320;
	}
	if ( (*p) < 39 ) {
		if ( 32 <= (*p) && (*p) <= 37 )
			goto tr320;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr320;
		} else if ( (*p) >= 63 )
			goto tr320;
	} else
		goto tr320;
	goto tr294;
tr320:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st20;
st20:
	if ( ++p == pe )
		goto _out20;
case 20:
#line 630 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 9: goto st20;
		case 34: goto tr319;
		case 61: goto st20;
		case 95: goto st20;
	}
	if ( (*p) < 39 ) {
		if ( 32 <= (*p) && (*p) <= 37 )
			goto st20;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st20;
		} else if ( (*p) >= 63 )
			goto st20;
	} else
		goto st20;
	goto tr294;
tr319:
#line 62 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("public_id"), aval); }
	goto st21;
st21:
	if ( ++p == pe )
		goto _out21;
case 21:
#line 657 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st22;
		case 62: goto tr300;
		case 91: goto st26;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st22;
	goto tr294;
st22:
	if ( ++p == pe )
		goto _out22;
case 22:
	switch( (*p) ) {
		case 32: goto st22;
		case 34: goto st23;
		case 39: goto st28;
		case 62: goto tr300;
		case 91: goto st26;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st22;
	goto tr294;
st23:
	if ( ++p == pe )
		goto _out23;
case 23:
	if ( (*p) == 34 )
		goto tr5;
	goto tr222;
tr222:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st24;
st24:
	if ( ++p == pe )
		goto _out24;
case 24:
#line 695 "ext/hpricot_scan/hpricot_scan.c"
	if ( (*p) == 34 )
		goto tr5;
	goto st24;
tr5:
#line 63 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("system_id"), aval); }
	goto st25;
st25:
	if ( ++p == pe )
		goto _out25;
case 25:
#line 707 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st25;
		case 62: goto tr300;
		case 91: goto st26;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st25;
	goto tr64;
tr330:
#line 55 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(tag, p); }
	goto st26;
st26:
	if ( ++p == pe )
		goto _out26;
case 26:
#line 724 "ext/hpricot_scan/hpricot_scan.c"
	if ( (*p) == 93 )
		goto st27;
	goto st26;
st27:
	if ( ++p == pe )
		goto _out27;
case 27:
	switch( (*p) ) {
		case 32: goto st27;
		case 62: goto tr300;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st27;
	goto tr64;
st28:
	if ( ++p == pe )
		goto _out28;
case 28:
	if ( (*p) == 39 )
		goto tr5;
	goto tr202;
tr202:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st29;
st29:
	if ( ++p == pe )
		goto _out29;
case 29:
#line 754 "ext/hpricot_scan/hpricot_scan.c"
	if ( (*p) == 39 )
		goto tr5;
	goto st29;
st30:
	if ( ++p == pe )
		goto _out30;
case 30:
	switch( (*p) ) {
		case 9: goto tr321;
		case 39: goto tr322;
		case 61: goto tr321;
		case 95: goto tr321;
	}
	if ( (*p) < 40 ) {
		if ( (*p) > 33 ) {
			if ( 35 <= (*p) && (*p) <= 37 )
				goto tr321;
		} else if ( (*p) >= 32 )
			goto tr321;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr321;
		} else if ( (*p) >= 63 )
			goto tr321;
	} else
		goto tr321;
	goto tr294;
tr321:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st31;
st31:
	if ( ++p == pe )
		goto _out31;
case 31:
#line 791 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 9: goto st31;
		case 39: goto tr301;
		case 61: goto st31;
		case 95: goto st31;
	}
	if ( (*p) < 40 ) {
		if ( (*p) > 33 ) {
			if ( 35 <= (*p) && (*p) <= 37 )
				goto st31;
		} else if ( (*p) >= 32 )
			goto st31;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st31;
		} else if ( (*p) >= 63 )
			goto st31;
	} else
		goto st31;
	goto tr294;
tr39:
#line 62 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("public_id"), aval); }
#line 63 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("system_id"), aval); }
	goto st32;
tr301:
#line 62 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("public_id"), aval); }
	goto st32;
tr322:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 62 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("public_id"), aval); }
	goto st32;
st32:
	if ( ++p == pe )
		goto _out32;
case 32:
#line 833 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 9: goto st33;
		case 32: goto st33;
		case 33: goto st31;
		case 39: goto tr301;
		case 62: goto tr300;
		case 91: goto st26;
		case 95: goto st31;
	}
	if ( (*p) < 40 ) {
		if ( (*p) > 13 ) {
			if ( 35 <= (*p) && (*p) <= 37 )
				goto st31;
		} else if ( (*p) >= 10 )
			goto st22;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st31;
		} else if ( (*p) >= 61 )
			goto st31;
	} else
		goto st31;
	goto tr294;
st33:
	if ( ++p == pe )
		goto _out33;
case 33:
	switch( (*p) ) {
		case 9: goto st33;
		case 32: goto st33;
		case 34: goto st23;
		case 39: goto tr299;
		case 62: goto tr300;
		case 91: goto st26;
		case 95: goto st31;
	}
	if ( (*p) < 40 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 37 )
				goto st31;
		} else if ( (*p) >= 10 )
			goto st22;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st31;
		} else if ( (*p) >= 61 )
			goto st31;
	} else
		goto st31;
	goto tr294;
tr41:
#line 62 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("public_id"), aval); }
#line 63 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("system_id"), aval); }
	goto st34;
tr299:
#line 62 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("public_id"), aval); }
	goto st34;
st34:
	if ( ++p == pe )
		goto _out34;
case 34:
#line 900 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 9: goto tr206;
		case 32: goto tr206;
		case 33: goto tr208;
		case 39: goto tr39;
		case 62: goto tr204;
		case 91: goto tr205;
		case 95: goto tr208;
	}
	if ( (*p) < 40 ) {
		if ( (*p) > 13 ) {
			if ( 35 <= (*p) && (*p) <= 37 )
				goto tr208;
		} else if ( (*p) >= 10 )
			goto tr207;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr208;
		} else if ( (*p) >= 61 )
			goto tr208;
	} else
		goto tr208;
	goto tr202;
tr206:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st35;
st35:
	if ( ++p == pe )
		goto _out35;
case 35:
#line 933 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 9: goto st35;
		case 32: goto st35;
		case 34: goto st37;
		case 39: goto tr41;
		case 62: goto tr37;
		case 91: goto st40;
		case 95: goto st47;
	}
	if ( (*p) < 40 ) {
		if ( (*p) > 13 ) {
			if ( 33 <= (*p) && (*p) <= 37 )
				goto st47;
		} else if ( (*p) >= 10 )
			goto st36;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st47;
		} else if ( (*p) >= 61 )
			goto st47;
	} else
		goto st47;
	goto st29;
tr207:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st36;
st36:
	if ( ++p == pe )
		goto _out36;
case 36:
#line 966 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st36;
		case 34: goto st37;
		case 39: goto tr36;
		case 62: goto tr37;
		case 91: goto st40;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st36;
	goto st29;
st37:
	if ( ++p == pe )
		goto _out37;
case 37:
	switch( (*p) ) {
		case 34: goto tr59;
		case 39: goto tr224;
	}
	goto tr223;
tr223:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st38;
st38:
	if ( ++p == pe )
		goto _out38;
case 38:
#line 994 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 34: goto tr59;
		case 39: goto tr60;
	}
	goto st38;
tr59:
#line 63 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("system_id"), aval); }
	goto st39;
tr203:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st39;
st39:
	if ( ++p == pe )
		goto _out39;
case 39:
#line 1012 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st39;
		case 39: goto tr5;
		case 62: goto tr37;
		case 91: goto st40;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st39;
	goto st29;
tr37:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 127 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 2;}
	goto st215;
tr204:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 127 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 2;}
	goto st215;
st215:
	if ( ++p == pe )
		goto _out215;
case 215:
#line 1040 "ext/hpricot_scan/hpricot_scan.c"
	if ( (*p) == 39 )
		goto tr5;
	goto st29;
tr205:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st40;
st40:
	if ( ++p == pe )
		goto _out40;
case 40:
#line 1052 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 39: goto tr32;
		case 93: goto st42;
	}
	goto st40;
tr32:
#line 63 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("system_id"), aval); }
	goto st41;
st41:
	if ( ++p == pe )
		goto _out41;
case 41:
#line 1066 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st41;
		case 62: goto tr24;
		case 93: goto st27;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st41;
	goto st26;
tr24:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 127 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 2;}
	goto st216;
st216:
	if ( ++p == pe )
		goto _out216;
case 216:
#line 1085 "ext/hpricot_scan/hpricot_scan.c"
	if ( (*p) == 93 )
		goto st27;
	goto st26;
st42:
	if ( ++p == pe )
		goto _out42;
case 42:
	switch( (*p) ) {
		case 32: goto st42;
		case 39: goto tr5;
		case 62: goto tr37;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st42;
	goto st29;
tr60:
#line 63 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("system_id"), aval); }
	goto st43;
tr224:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 63 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("system_id"), aval); }
	goto st43;
st43:
	if ( ++p == pe )
		goto _out43;
case 43:
#line 1115 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st43;
		case 34: goto tr5;
		case 62: goto tr57;
		case 91: goto st44;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st43;
	goto st24;
tr57:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 127 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 2;}
	goto st217;
st217:
	if ( ++p == pe )
		goto _out217;
case 217:
#line 1135 "ext/hpricot_scan/hpricot_scan.c"
	if ( (*p) == 34 )
		goto tr5;
	goto st24;
st44:
	if ( ++p == pe )
		goto _out44;
case 44:
	switch( (*p) ) {
		case 34: goto tr32;
		case 93: goto st45;
	}
	goto st44;
st45:
	if ( ++p == pe )
		goto _out45;
case 45:
	switch( (*p) ) {
		case 32: goto st45;
		case 34: goto tr5;
		case 62: goto tr57;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st45;
	goto st24;
tr36:
#line 63 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("system_id"), aval); }
	goto st46;
st46:
	if ( ++p == pe )
		goto _out46;
case 46:
#line 1168 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr203;
		case 39: goto tr5;
		case 62: goto tr204;
		case 91: goto tr205;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr203;
	goto tr202;
tr208:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st47;
st47:
	if ( ++p == pe )
		goto _out47;
case 47:
#line 1186 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 9: goto st47;
		case 39: goto tr39;
		case 61: goto st47;
		case 95: goto st47;
	}
	if ( (*p) < 40 ) {
		if ( (*p) > 33 ) {
			if ( 35 <= (*p) && (*p) <= 37 )
				goto st47;
		} else if ( (*p) >= 32 )
			goto st47;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st47;
		} else if ( (*p) >= 63 )
			goto st47;
	} else
		goto st47;
	goto st29;
st48:
	if ( ++p == pe )
		goto _out48;
case 48:
	if ( (*p) == 89 )
		goto st49;
	goto tr294;
st49:
	if ( ++p == pe )
		goto _out49;
case 49:
	if ( (*p) == 83 )
		goto st50;
	goto tr294;
st50:
	if ( ++p == pe )
		goto _out50;
case 50:
	if ( (*p) == 84 )
		goto st51;
	goto tr294;
st51:
	if ( ++p == pe )
		goto _out51;
case 51:
	if ( (*p) == 69 )
		goto st52;
	goto tr294;
st52:
	if ( ++p == pe )
		goto _out52;
case 52:
	if ( (*p) == 77 )
		goto st21;
	goto tr294;
st53:
	if ( ++p == pe )
		goto _out53;
case 53:
	if ( (*p) == 67 )
		goto st54;
	goto tr294;
st54:
	if ( ++p == pe )
		goto _out54;
case 54:
	if ( (*p) == 68 )
		goto st55;
	goto tr294;
st55:
	if ( ++p == pe )
		goto _out55;
case 55:
	if ( (*p) == 65 )
		goto st56;
	goto tr294;
st56:
	if ( ++p == pe )
		goto _out56;
case 56:
	if ( (*p) == 84 )
		goto st57;
	goto tr294;
st57:
	if ( ++p == pe )
		goto _out57;
case 57:
	if ( (*p) == 65 )
		goto st58;
	goto tr294;
st58:
	if ( ++p == pe )
		goto _out58;
case 58:
	if ( (*p) == 91 )
		goto tr354;
	goto tr294;
st59:
	if ( ++p == pe )
		goto _out59;
case 59:
	switch( (*p) ) {
		case 58: goto tr339;
		case 95: goto tr339;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr339;
	} else if ( (*p) >= 65 )
		goto tr339;
	goto tr294;
tr339:
#line 52 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_tag = p; }
	goto st60;
st60:
	if ( ++p == pe )
		goto _out60;
case 60:
#line 1307 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr331;
		case 62: goto tr333;
		case 95: goto st60;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st60;
		} else if ( (*p) >= 9 )
			goto tr331;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st60;
		} else if ( (*p) >= 65 )
			goto st60;
	} else
		goto st60;
	goto tr294;
tr331:
#line 55 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(tag, p); }
	goto st61;
st61:
	if ( ++p == pe )
		goto _out61;
case 61:
#line 1336 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st61;
		case 62: goto tr312;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st61;
	goto tr294;
tr18:
#line 52 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_tag = p; }
	goto st62;
st62:
	if ( ++p == pe )
		goto _out62;
case 62:
#line 1352 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr323;
		case 47: goto tr325;
		case 62: goto tr326;
		case 95: goto st62;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr323;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st62;
		} else if ( (*p) >= 65 )
			goto st62;
	} else
		goto st62;
	goto tr294;
tr323:
#line 55 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(tag, p); }
	goto st63;
st63:
	if ( ++p == pe )
		goto _out63;
case 63:
#line 1379 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st63;
		case 47: goto st66;
		case 58: goto tr316;
		case 62: goto tr317;
		case 95: goto tr316;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st63;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr316;
		} else if ( (*p) >= 65 )
			goto tr316;
	} else
		goto tr314;
	goto tr294;
tr359:
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st64;
tr314:
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st64;
st64:
	if ( ++p == pe )
		goto _out64;
case 64:
#line 1429 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr368;
		case 47: goto tr365;
		case 62: goto tr367;
		case 95: goto st64;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr368;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st64;
		} else if ( (*p) >= 65 )
			goto st64;
	} else
		goto st64;
	goto tr64;
tr3:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st65;
tr368:
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st65;
tr119:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st65;
st65:
	if ( ++p == pe )
		goto _out65;
case 65:
#line 1466 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st65;
		case 47: goto tr360;
		case 58: goto tr361;
		case 62: goto tr165;
		case 95: goto tr361;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st65;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr361;
		} else if ( (*p) >= 65 )
			goto tr361;
	} else
		goto tr359;
	goto tr64;
tr360:
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st66;
tr365:
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st66;
st66:
	if ( ++p == pe )
		goto _out66;
case 66:
#line 1504 "ext/hpricot_scan/hpricot_scan.c"
	if ( (*p) == 62 )
		goto tr355;
	goto tr64;
tr361:
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st67;
tr316:
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st67;
st67:
	if ( ++p == pe )
		goto _out67;
case 67:
#line 1538 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr363;
		case 47: goto tr365;
		case 61: goto tr366;
		case 62: goto tr367;
		case 95: goto st67;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr363;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st67;
		} else if ( (*p) >= 65 )
			goto st67;
	} else
		goto st67;
	goto tr64;
tr66:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st68;
tr363:
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st68;
tr139:
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st68;
st68:
	if ( ++p == pe )
		goto _out68;
case 68:
#line 1576 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st68;
		case 47: goto tr360;
		case 58: goto tr361;
		case 61: goto st69;
		case 62: goto tr165;
		case 95: goto tr361;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st68;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr361;
		} else if ( (*p) >= 65 )
			goto tr361;
	} else
		goto tr359;
	goto tr64;
tr366:
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st69;
st69:
	if ( ++p == pe )
		goto _out69;
case 69:
#line 1605 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st69;
		case 34: goto st141;
		case 39: goto st142;
		case 60: goto tr64;
		case 62: goto tr64;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 13 )
			goto tr157;
	} else if ( (*p) >= 9 )
		goto st69;
	goto tr155;
tr155:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st70;
st70:
	if ( ++p == pe )
		goto _out70;
case 70:
#line 1627 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr3;
		case 47: goto tr63;
		case 60: goto tr64;
		case 62: goto tr65;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 13 )
			goto tr62;
	} else if ( (*p) >= 9 )
		goto tr3;
	goto st70;
tr62:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st71;
tr120:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st71;
st71:
	if ( ++p == pe )
		goto _out71;
case 71:
#line 1654 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr3;
		case 47: goto tr69;
		case 58: goto tr70;
		case 60: goto tr64;
		case 62: goto tr72;
		case 95: goto tr70;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 13 )
				goto tr62;
		} else if ( (*p) >= 9 )
			goto tr3;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr70;
		} else if ( (*p) >= 65 )
			goto tr70;
	} else
		goto tr68;
	goto st70;
tr68:
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st72;
tr162:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st72;
st72:
	if ( ++p == pe )
		goto _out72;
case 72:
#line 1714 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr119;
		case 47: goto tr122;
		case 60: goto tr64;
		case 62: goto tr123;
		case 95: goto st72;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 13 )
				goto tr120;
		} else if ( (*p) >= 9 )
			goto tr119;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st72;
		} else if ( (*p) >= 65 )
			goto st72;
	} else
		goto st72;
	goto st70;
tr63:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st73;
tr69:
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st73;
tr122:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st73;
tr163:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st73;
tr230:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st73;
tr231:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st73;
st73:
	if ( ++p == pe )
		goto _out73;
case 73:
#line 1795 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr3;
		case 47: goto tr63;
		case 60: goto tr64;
		case 62: goto tr65;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 13 )
			goto tr62;
	} else if ( (*p) >= 9 )
		goto tr3;
	goto st70;
tr70:
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st74;
tr164:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st74;
st74:
	if ( ++p == pe )
		goto _out74;
case 74:
#line 1844 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr139;
		case 47: goto tr122;
		case 60: goto tr64;
		case 61: goto tr142;
		case 62: goto tr123;
		case 95: goto st74;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 13 )
				goto tr140;
		} else if ( (*p) >= 9 )
			goto tr139;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st74;
		} else if ( (*p) >= 65 )
			goto st74;
	} else
		goto st74;
	goto st70;
tr67:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st75;
tr140:
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st75;
st75:
	if ( ++p == pe )
		goto _out75;
case 75:
#line 1882 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr66;
		case 47: goto tr69;
		case 58: goto tr70;
		case 60: goto tr64;
		case 61: goto st76;
		case 62: goto tr72;
		case 95: goto tr70;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 13 )
				goto tr67;
		} else if ( (*p) >= 9 )
			goto tr66;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr70;
		} else if ( (*p) >= 65 )
			goto tr70;
	} else
		goto tr68;
	goto st70;
tr142:
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st76;
tr157:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st76;
st76:
	if ( ++p == pe )
		goto _out76;
case 76:
#line 1919 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr226;
		case 34: goto st79;
		case 39: goto st140;
		case 47: goto tr230;
		case 60: goto tr64;
		case 62: goto tr65;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 13 )
			goto tr227;
	} else if ( (*p) >= 9 )
		goto tr226;
	goto tr155;
tr226:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st77;
st77:
	if ( ++p == pe )
		goto _out77;
case 77:
#line 1942 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st77;
		case 34: goto st141;
		case 39: goto st142;
		case 47: goto tr163;
		case 58: goto tr164;
		case 60: goto tr64;
		case 62: goto tr165;
		case 95: goto tr164;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 13 )
				goto tr161;
		} else if ( (*p) >= 9 )
			goto st77;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr164;
		} else if ( (*p) >= 65 )
			goto tr164;
	} else
		goto tr162;
	goto tr155;
tr161:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st78;
tr227:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st78;
st78:
	if ( ++p == pe )
		goto _out78;
case 78:
#line 1982 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr226;
		case 34: goto st79;
		case 39: goto st140;
		case 47: goto tr231;
		case 58: goto tr164;
		case 60: goto tr64;
		case 62: goto tr72;
		case 95: goto tr164;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 13 )
				goto tr227;
		} else if ( (*p) >= 9 )
			goto tr226;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr164;
		} else if ( (*p) >= 65 )
			goto tr164;
	} else
		goto tr162;
	goto tr155;
st79:
	if ( ++p == pe )
		goto _out79;
case 79:
	switch( (*p) ) {
		case 32: goto tr216;
		case 34: goto tr62;
		case 47: goto tr242;
		case 60: goto tr209;
		case 62: goto tr255;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 13 )
			goto tr254;
	} else if ( (*p) >= 9 )
		goto tr216;
	goto tr176;
tr176:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st80;
st80:
	if ( ++p == pe )
		goto _out80;
case 80:
#line 2033 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr9;
		case 34: goto tr62;
		case 47: goto tr83;
		case 60: goto st82;
		case 62: goto tr84;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 13 )
			goto tr82;
	} else if ( (*p) >= 9 )
		goto tr9;
	goto st80;
tr9:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st81;
tr105:
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st81;
tr129:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st81;
tr210:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st81;
tr216:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st81;
st81:
	if ( ++p == pe )
		goto _out81;
case 81:
#line 2075 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st81;
		case 34: goto tr3;
		case 47: goto tr45;
		case 58: goto tr46;
		case 62: goto tr47;
		case 95: goto tr46;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st81;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr46;
		} else if ( (*p) >= 65 )
			goto tr46;
	} else
		goto tr44;
	goto st82;
tr209:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st82;
st82:
	if ( ++p == pe )
		goto _out82;
case 82:
#line 2104 "ext/hpricot_scan/hpricot_scan.c"
	if ( (*p) == 34 )
		goto tr3;
	goto st82;
tr44:
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st83;
tr211:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st83;
st83:
	if ( ++p == pe )
		goto _out83;
case 83:
#line 2144 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr105;
		case 34: goto tr3;
		case 47: goto tr107;
		case 62: goto tr108;
		case 95: goto st83;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr105;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st83;
		} else if ( (*p) >= 65 )
			goto st83;
	} else
		goto st83;
	goto st82;
tr45:
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st84;
tr107:
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st84;
tr212:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st84;
st84:
	if ( ++p == pe )
		goto _out84;
case 84:
#line 2190 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 34: goto tr3;
		case 62: goto tr42;
	}
	goto st82;
tr42:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 131 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 6;}
	goto st218;
tr47:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 4;}
	goto st218;
tr84:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 4;}
	goto st218;
tr88:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 4;}
	goto st218;
tr108:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 4;}
	goto st218;
tr133:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 4;}
	goto st218;
tr214:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 4;}
	goto st218;
tr255:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 4;}
	goto st218;
tr256:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 4;}
	goto st218;
st218:
	if ( ++p == pe )
		goto _out218;
case 218:
#line 2306 "ext/hpricot_scan/hpricot_scan.c"
	if ( (*p) == 34 )
		goto tr3;
	goto st82;
tr46:
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st85;
tr213:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st85;
st85:
	if ( ++p == pe )
		goto _out85;
case 85:
#line 2346 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr109;
		case 34: goto tr3;
		case 47: goto tr107;
		case 61: goto tr111;
		case 62: goto tr108;
		case 95: goto st85;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr109;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st85;
		} else if ( (*p) >= 65 )
			goto st85;
	} else
		goto st85;
	goto st82;
tr377:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st86;
tr109:
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st86;
tr147:
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st86;
st86:
	if ( ++p == pe )
		goto _out86;
case 86:
#line 2385 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st86;
		case 34: goto tr3;
		case 47: goto tr45;
		case 58: goto tr46;
		case 61: goto st87;
		case 62: goto tr47;
		case 95: goto tr46;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st86;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr46;
		} else if ( (*p) >= 65 )
			goto tr46;
	} else
		goto tr44;
	goto st82;
tr111:
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st87;
st87:
	if ( ++p == pe )
		goto _out87;
case 87:
#line 2415 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st87;
		case 34: goto tr179;
		case 39: goto st139;
		case 60: goto st82;
		case 62: goto st82;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 13 )
			goto tr178;
	} else if ( (*p) >= 9 )
		goto st87;
	goto tr176;
tr150:
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st88;
tr178:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st88;
st88:
	if ( ++p == pe )
		goto _out88;
case 88:
#line 2441 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr238;
		case 34: goto tr240;
		case 39: goto st97;
		case 47: goto tr242;
		case 60: goto st82;
		case 62: goto tr84;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 13 )
			goto tr239;
	} else if ( (*p) >= 9 )
		goto tr238;
	goto tr176;
tr238:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st89;
st89:
	if ( ++p == pe )
		goto _out89;
case 89:
#line 2464 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st89;
		case 34: goto tr179;
		case 39: goto st139;
		case 47: goto tr184;
		case 58: goto tr185;
		case 60: goto st82;
		case 62: goto tr47;
		case 95: goto tr185;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 13 )
				goto tr182;
		} else if ( (*p) >= 9 )
			goto st89;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr185;
		} else if ( (*p) >= 65 )
			goto tr185;
	} else
		goto tr183;
	goto tr176;
tr182:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st90;
tr239:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st90;
st90:
	if ( ++p == pe )
		goto _out90;
case 90:
#line 2504 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr238;
		case 34: goto tr240;
		case 39: goto st97;
		case 47: goto tr243;
		case 58: goto tr185;
		case 60: goto st82;
		case 62: goto tr88;
		case 95: goto tr185;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 13 )
				goto tr239;
		} else if ( (*p) >= 9 )
			goto tr238;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr185;
		} else if ( (*p) >= 65 )
			goto tr185;
	} else
		goto tr183;
	goto tr176;
tr240:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st91;
st91:
	if ( ++p == pe )
		goto _out91;
case 91:
#line 2538 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr216;
		case 34: goto tr62;
		case 47: goto tr243;
		case 58: goto tr185;
		case 60: goto tr209;
		case 62: goto tr256;
		case 95: goto tr185;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 13 )
				goto tr254;
		} else if ( (*p) >= 9 )
			goto tr216;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr185;
		} else if ( (*p) >= 65 )
			goto tr185;
	} else
		goto tr183;
	goto tr176;
tr82:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st92;
tr130:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st92;
tr254:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st92;
st92:
	if ( ++p == pe )
		goto _out92;
case 92:
#line 2583 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr9;
		case 34: goto tr62;
		case 47: goto tr86;
		case 58: goto tr87;
		case 60: goto st82;
		case 62: goto tr88;
		case 95: goto tr87;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 13 )
				goto tr82;
		} else if ( (*p) >= 9 )
			goto tr9;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr87;
		} else if ( (*p) >= 65 )
			goto tr87;
	} else
		goto tr85;
	goto st80;
tr85:
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st93;
tr183:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st93;
st93:
	if ( ++p == pe )
		goto _out93;
case 93:
#line 2644 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr129;
		case 34: goto tr62;
		case 47: goto tr132;
		case 60: goto st82;
		case 62: goto tr133;
		case 95: goto st93;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 13 )
				goto tr130;
		} else if ( (*p) >= 9 )
			goto tr129;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st93;
		} else if ( (*p) >= 65 )
			goto st93;
	} else
		goto st93;
	goto st80;
tr83:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st94;
tr86:
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st94;
tr132:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st94;
tr184:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st94;
tr242:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st94;
tr243:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st94;
st94:
	if ( ++p == pe )
		goto _out94;
case 94:
#line 2726 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr9;
		case 34: goto tr62;
		case 47: goto tr83;
		case 60: goto st82;
		case 62: goto tr84;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 13 )
			goto tr82;
	} else if ( (*p) >= 9 )
		goto tr9;
	goto st80;
tr87:
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st95;
tr185:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st95;
st95:
	if ( ++p == pe )
		goto _out95;
case 95:
#line 2776 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr147;
		case 34: goto tr62;
		case 47: goto tr132;
		case 60: goto st82;
		case 61: goto tr150;
		case 62: goto tr133;
		case 95: goto st95;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 13 )
				goto tr148;
		} else if ( (*p) >= 9 )
			goto tr147;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st95;
		} else if ( (*p) >= 65 )
			goto st95;
	} else
		goto st95;
	goto st80;
tr378:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st96;
tr148:
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st96;
st96:
	if ( ++p == pe )
		goto _out96;
case 96:
#line 2815 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr377;
		case 34: goto tr62;
		case 47: goto tr86;
		case 58: goto tr87;
		case 60: goto st82;
		case 61: goto st88;
		case 62: goto tr88;
		case 95: goto tr87;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 13 )
				goto tr378;
		} else if ( (*p) >= 9 )
			goto tr377;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr87;
		} else if ( (*p) >= 65 )
			goto tr87;
	} else
		goto tr85;
	goto st80;
st97:
	if ( ++p == pe )
		goto _out97;
case 97:
	switch( (*p) ) {
		case 32: goto tr257;
		case 34: goto tr261;
		case 39: goto tr82;
		case 47: goto tr248;
		case 60: goto tr215;
		case 62: goto tr259;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 13 )
			goto tr258;
	} else if ( (*p) >= 9 )
		goto tr257;
	goto tr186;
tr186:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st98;
st98:
	if ( ++p == pe )
		goto _out98;
case 98:
#line 2867 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr90;
		case 34: goto tr74;
		case 39: goto tr82;
		case 47: goto tr92;
		case 60: goto st100;
		case 62: goto tr93;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 13 )
			goto tr91;
	} else if ( (*p) >= 9 )
		goto tr90;
	goto st98;
tr90:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st99;
tr112:
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st99;
tr134:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st99;
tr217:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st99;
tr257:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st99;
st99:
	if ( ++p == pe )
		goto _out99;
case 99:
#line 2910 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st99;
		case 34: goto tr8;
		case 39: goto tr9;
		case 47: goto tr51;
		case 58: goto tr52;
		case 62: goto tr53;
		case 95: goto tr52;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st99;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr52;
		} else if ( (*p) >= 65 )
			goto tr52;
	} else
		goto tr50;
	goto st100;
tr215:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st100;
st100:
	if ( ++p == pe )
		goto _out100;
case 100:
#line 2940 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 34: goto tr8;
		case 39: goto tr9;
	}
	goto st100;
tr8:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st101;
tr98:
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st101;
tr124:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st101;
tr197:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st101;
tr250:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st101;
tr225:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st101;
st101:
	if ( ++p == pe )
		goto _out101;
case 101:
#line 2980 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st101;
		case 39: goto tr3;
		case 47: goto tr28;
		case 58: goto tr29;
		case 62: goto tr30;
		case 95: goto tr29;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st101;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr29;
		} else if ( (*p) >= 65 )
			goto tr29;
	} else
		goto tr27;
	goto st102;
tr196:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st102;
st102:
	if ( ++p == pe )
		goto _out102;
case 102:
#line 3009 "ext/hpricot_scan/hpricot_scan.c"
	if ( (*p) == 39 )
		goto tr3;
	goto st102;
tr27:
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st103;
tr198:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st103;
st103:
	if ( ++p == pe )
		goto _out103;
case 103:
#line 3049 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr98;
		case 39: goto tr3;
		case 47: goto tr100;
		case 62: goto tr101;
		case 95: goto st103;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr98;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st103;
		} else if ( (*p) >= 65 )
			goto st103;
	} else
		goto st103;
	goto st102;
tr28:
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st104;
tr100:
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st104;
tr199:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st104;
st104:
	if ( ++p == pe )
		goto _out104;
case 104:
#line 3095 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 39: goto tr3;
		case 62: goto tr25;
	}
	goto st102;
tr25:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 131 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 6;}
	goto st219;
tr30:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 4;}
	goto st219;
tr76:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 4;}
	goto st219;
tr80:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 4;}
	goto st219;
tr101:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 4;}
	goto st219;
tr128:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 4;}
	goto st219;
tr201:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 4;}
	goto st219;
tr252:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 4;}
	goto st219;
tr253:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 4;}
	goto st219;
st219:
	if ( ++p == pe )
		goto _out219;
case 219:
#line 3211 "ext/hpricot_scan/hpricot_scan.c"
	if ( (*p) == 39 )
		goto tr3;
	goto st102;
tr29:
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st105;
tr200:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st105;
st105:
	if ( ++p == pe )
		goto _out105;
case 105:
#line 3251 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr102;
		case 39: goto tr3;
		case 47: goto tr100;
		case 61: goto tr104;
		case 62: goto tr101;
		case 95: goto st105;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr102;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st105;
		} else if ( (*p) >= 65 )
			goto st105;
	} else
		goto st105;
	goto st102;
tr374:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st106;
tr102:
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st106;
tr143:
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st106;
st106:
	if ( ++p == pe )
		goto _out106;
case 106:
#line 3290 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st106;
		case 39: goto tr3;
		case 47: goto tr28;
		case 58: goto tr29;
		case 61: goto st107;
		case 62: goto tr30;
		case 95: goto tr29;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st106;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr29;
		} else if ( (*p) >= 65 )
			goto tr29;
	} else
		goto tr27;
	goto st102;
tr104:
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st107;
st107:
	if ( ++p == pe )
		goto _out107;
case 107:
#line 3320 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st107;
		case 34: goto st136;
		case 39: goto tr170;
		case 60: goto st102;
		case 62: goto st102;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 13 )
			goto tr168;
	} else if ( (*p) >= 9 )
		goto st107;
	goto tr166;
tr166:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st108;
st108:
	if ( ++p == pe )
		goto _out108;
case 108:
#line 3342 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr8;
		case 39: goto tr62;
		case 47: goto tr75;
		case 60: goto st102;
		case 62: goto tr76;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 13 )
			goto tr74;
	} else if ( (*p) >= 9 )
		goto tr8;
	goto st108;
tr74:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st109;
tr125:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st109;
tr251:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st109;
tr261:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st109;
st109:
	if ( ++p == pe )
		goto _out109;
case 109:
#line 3382 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr8;
		case 39: goto tr62;
		case 47: goto tr78;
		case 58: goto tr79;
		case 60: goto st102;
		case 62: goto tr80;
		case 95: goto tr79;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 13 )
				goto tr74;
		} else if ( (*p) >= 9 )
			goto tr8;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr79;
		} else if ( (*p) >= 65 )
			goto tr79;
	} else
		goto tr77;
	goto st108;
tr77:
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st110;
tr173:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st110;
st110:
	if ( ++p == pe )
		goto _out110;
case 110:
#line 3443 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr124;
		case 39: goto tr62;
		case 47: goto tr127;
		case 60: goto st102;
		case 62: goto tr128;
		case 95: goto st110;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 13 )
				goto tr125;
		} else if ( (*p) >= 9 )
			goto tr124;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st110;
		} else if ( (*p) >= 65 )
			goto st110;
	} else
		goto st110;
	goto st108;
tr75:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st111;
tr78:
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st111;
tr127:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st111;
tr174:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st111;
tr236:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st111;
tr237:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st111;
st111:
	if ( ++p == pe )
		goto _out111;
case 111:
#line 3525 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr8;
		case 39: goto tr62;
		case 47: goto tr75;
		case 60: goto st102;
		case 62: goto tr76;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 13 )
			goto tr74;
	} else if ( (*p) >= 9 )
		goto tr8;
	goto st108;
tr79:
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st112;
tr175:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st112;
st112:
	if ( ++p == pe )
		goto _out112;
case 112:
#line 3575 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr143;
		case 39: goto tr62;
		case 47: goto tr127;
		case 60: goto st102;
		case 61: goto tr146;
		case 62: goto tr128;
		case 95: goto st112;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 13 )
				goto tr144;
		} else if ( (*p) >= 9 )
			goto tr143;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st112;
		} else if ( (*p) >= 65 )
			goto st112;
	} else
		goto st112;
	goto st108;
tr375:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st113;
tr144:
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st113;
st113:
	if ( ++p == pe )
		goto _out113;
case 113:
#line 3614 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr374;
		case 39: goto tr62;
		case 47: goto tr78;
		case 58: goto tr79;
		case 60: goto st102;
		case 61: goto st114;
		case 62: goto tr80;
		case 95: goto tr79;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 13 )
				goto tr375;
		} else if ( (*p) >= 9 )
			goto tr374;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr79;
		} else if ( (*p) >= 65 )
			goto tr79;
	} else
		goto tr77;
	goto st108;
tr146:
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st114;
tr168:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st114;
st114:
	if ( ++p == pe )
		goto _out114;
case 114:
#line 3652 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr232;
		case 34: goto st117;
		case 39: goto tr235;
		case 47: goto tr236;
		case 60: goto st102;
		case 62: goto tr76;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 13 )
			goto tr233;
	} else if ( (*p) >= 9 )
		goto tr232;
	goto tr166;
tr232:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st115;
st115:
	if ( ++p == pe )
		goto _out115;
case 115:
#line 3675 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st115;
		case 34: goto st136;
		case 39: goto tr170;
		case 47: goto tr174;
		case 58: goto tr175;
		case 60: goto st102;
		case 62: goto tr30;
		case 95: goto tr175;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 13 )
				goto tr172;
		} else if ( (*p) >= 9 )
			goto st115;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr175;
		} else if ( (*p) >= 65 )
			goto tr175;
	} else
		goto tr173;
	goto tr166;
tr172:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st116;
tr233:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st116;
st116:
	if ( ++p == pe )
		goto _out116;
case 116:
#line 3715 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr232;
		case 34: goto st117;
		case 39: goto tr235;
		case 47: goto tr237;
		case 58: goto tr175;
		case 60: goto st102;
		case 62: goto tr80;
		case 95: goto tr175;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 13 )
				goto tr233;
		} else if ( (*p) >= 9 )
			goto tr232;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr175;
		} else if ( (*p) >= 65 )
			goto tr175;
	} else
		goto tr173;
	goto tr166;
st117:
	if ( ++p == pe )
		goto _out117;
case 117:
	switch( (*p) ) {
		case 32: goto tr257;
		case 34: goto tr74;
		case 39: goto tr254;
		case 47: goto tr248;
		case 60: goto tr215;
		case 62: goto tr259;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 13 )
			goto tr258;
	} else if ( (*p) >= 9 )
		goto tr257;
	goto tr186;
tr91:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st118;
tr135:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st118;
tr258:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st118;
st118:
	if ( ++p == pe )
		goto _out118;
case 118:
#line 3779 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr90;
		case 34: goto tr74;
		case 39: goto tr82;
		case 47: goto tr95;
		case 58: goto tr96;
		case 60: goto st100;
		case 62: goto tr97;
		case 95: goto tr96;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 13 )
				goto tr91;
		} else if ( (*p) >= 9 )
			goto tr90;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr96;
		} else if ( (*p) >= 65 )
			goto tr96;
	} else
		goto tr94;
	goto st98;
tr94:
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st119;
tr193:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st119;
st119:
	if ( ++p == pe )
		goto _out119;
case 119:
#line 3841 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr134;
		case 34: goto tr74;
		case 39: goto tr82;
		case 47: goto tr137;
		case 60: goto st100;
		case 62: goto tr138;
		case 95: goto st119;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 13 )
				goto tr135;
		} else if ( (*p) >= 9 )
			goto tr134;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st119;
		} else if ( (*p) >= 65 )
			goto st119;
	} else
		goto st119;
	goto st98;
tr92:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st120;
tr95:
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st120;
tr137:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st120;
tr194:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st120;
tr248:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st120;
tr249:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st120;
st120:
	if ( ++p == pe )
		goto _out120;
case 120:
#line 3924 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr90;
		case 34: goto tr74;
		case 39: goto tr82;
		case 47: goto tr92;
		case 60: goto st100;
		case 62: goto tr93;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 13 )
			goto tr91;
	} else if ( (*p) >= 9 )
		goto tr90;
	goto st98;
tr48:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 131 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 6;}
	goto st220;
tr53:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 4;}
	goto st220;
tr93:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 4;}
	goto st220;
tr97:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 4;}
	goto st220;
tr115:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 4;}
	goto st220;
tr138:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 4;}
	goto st220;
tr221:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 4;}
	goto st220;
tr259:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 4;}
	goto st220;
tr260:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
#line 129 "ext/hpricot_scan/hpricot_scan.rl"
	{act = 4;}
	goto st220;
st220:
	if ( ++p == pe )
		goto _out220;
case 220:
#line 4049 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 34: goto tr8;
		case 39: goto tr9;
	}
	goto st100;
tr96:
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st121;
tr195:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st121;
st121:
	if ( ++p == pe )
		goto _out121;
case 121:
#line 4091 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr151;
		case 34: goto tr74;
		case 39: goto tr82;
		case 47: goto tr137;
		case 60: goto st100;
		case 61: goto tr154;
		case 62: goto tr138;
		case 95: goto st121;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 13 )
				goto tr152;
		} else if ( (*p) >= 9 )
			goto tr151;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st121;
		} else if ( (*p) >= 65 )
			goto st121;
	} else
		goto st121;
	goto st98;
tr380:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st122;
tr116:
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st122;
tr151:
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st122;
st122:
	if ( ++p == pe )
		goto _out122;
case 122:
#line 4135 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st122;
		case 34: goto tr8;
		case 39: goto tr9;
		case 47: goto tr51;
		case 58: goto tr52;
		case 61: goto st126;
		case 62: goto tr53;
		case 95: goto tr52;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st122;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr52;
		} else if ( (*p) >= 65 )
			goto tr52;
	} else
		goto tr50;
	goto st100;
tr50:
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st123;
tr218:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st123;
st123:
	if ( ++p == pe )
		goto _out123;
case 123:
#line 4194 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr112;
		case 34: goto tr8;
		case 39: goto tr9;
		case 47: goto tr114;
		case 62: goto tr115;
		case 95: goto st123;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr112;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st123;
		} else if ( (*p) >= 65 )
			goto st123;
	} else
		goto st123;
	goto st100;
tr51:
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st124;
tr114:
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st124;
tr219:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
	goto st124;
st124:
	if ( ++p == pe )
		goto _out124;
case 124:
#line 4241 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 34: goto tr8;
		case 39: goto tr9;
		case 62: goto tr48;
	}
	goto st100;
tr52:
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st125;
tr220:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 72 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    ATTR(akey, aval);
  }
#line 65 "ext/hpricot_scan/hpricot_scan.rl"
	{ 
    akey = Qnil;
    aval = Qnil;
    mark_akey = NULL;
    mark_aval = NULL;
  }
#line 54 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_akey = p; }
	goto st125;
st125:
	if ( ++p == pe )
		goto _out125;
case 125:
#line 4284 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr116;
		case 34: goto tr8;
		case 39: goto tr9;
		case 47: goto tr114;
		case 61: goto tr118;
		case 62: goto tr115;
		case 95: goto st125;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr116;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st125;
		} else if ( (*p) >= 65 )
			goto st125;
	} else
		goto st125;
	goto st100;
tr118:
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st126;
st126:
	if ( ++p == pe )
		goto _out126;
case 126:
#line 4314 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st126;
		case 34: goto tr189;
		case 39: goto tr190;
		case 60: goto st100;
		case 62: goto st100;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 13 )
			goto tr188;
	} else if ( (*p) >= 9 )
		goto st126;
	goto tr186;
tr154:
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
	goto st127;
tr188:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st127;
st127:
	if ( ++p == pe )
		goto _out127;
case 127:
#line 4340 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr244;
		case 34: goto tr246;
		case 39: goto tr247;
		case 47: goto tr248;
		case 60: goto st100;
		case 62: goto tr93;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 13 )
			goto tr245;
	} else if ( (*p) >= 9 )
		goto tr244;
	goto tr186;
tr244:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st128;
st128:
	if ( ++p == pe )
		goto _out128;
case 128:
#line 4363 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st128;
		case 34: goto tr189;
		case 39: goto tr190;
		case 47: goto tr194;
		case 58: goto tr195;
		case 60: goto st100;
		case 62: goto tr53;
		case 95: goto tr195;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 13 )
				goto tr192;
		} else if ( (*p) >= 9 )
			goto st128;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr195;
		} else if ( (*p) >= 65 )
			goto tr195;
	} else
		goto tr193;
	goto tr186;
tr192:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st129;
tr245:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st129;
st129:
	if ( ++p == pe )
		goto _out129;
case 129:
#line 4403 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr244;
		case 34: goto tr246;
		case 39: goto tr247;
		case 47: goto tr249;
		case 58: goto tr195;
		case 60: goto st100;
		case 62: goto tr97;
		case 95: goto tr195;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 13 )
				goto tr245;
		} else if ( (*p) >= 9 )
			goto tr244;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr195;
		} else if ( (*p) >= 65 )
			goto tr195;
	} else
		goto tr193;
	goto tr186;
tr246:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st130;
st130:
	if ( ++p == pe )
		goto _out130;
case 130:
#line 4437 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr257;
		case 34: goto tr74;
		case 39: goto tr254;
		case 47: goto tr249;
		case 58: goto tr195;
		case 60: goto tr215;
		case 62: goto tr260;
		case 95: goto tr195;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 13 )
				goto tr258;
		} else if ( (*p) >= 9 )
			goto tr257;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr195;
		} else if ( (*p) >= 65 )
			goto tr195;
	} else
		goto tr193;
	goto tr186;
tr247:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st131;
st131:
	if ( ++p == pe )
		goto _out131;
case 131:
#line 4471 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr257;
		case 34: goto tr261;
		case 39: goto tr82;
		case 47: goto tr249;
		case 58: goto tr195;
		case 60: goto tr215;
		case 62: goto tr260;
		case 95: goto tr195;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 13 )
				goto tr258;
		} else if ( (*p) >= 9 )
			goto tr257;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr195;
		} else if ( (*p) >= 65 )
			goto tr195;
	} else
		goto tr193;
	goto tr186;
tr189:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st132;
st132:
	if ( ++p == pe )
		goto _out132;
case 132:
#line 4505 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr217;
		case 34: goto tr8;
		case 39: goto tr216;
		case 47: goto tr219;
		case 58: goto tr220;
		case 62: goto tr221;
		case 95: goto tr220;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr217;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr220;
		} else if ( (*p) >= 65 )
			goto tr220;
	} else
		goto tr218;
	goto tr215;
tr190:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st133;
st133:
	if ( ++p == pe )
		goto _out133;
case 133:
#line 4535 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr217;
		case 34: goto tr225;
		case 39: goto tr9;
		case 47: goto tr219;
		case 58: goto tr220;
		case 62: goto tr221;
		case 95: goto tr220;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr217;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr220;
		} else if ( (*p) >= 65 )
			goto tr220;
	} else
		goto tr218;
	goto tr215;
tr381:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st134;
tr152:
#line 58 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(akey, p); }
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st134;
st134:
	if ( ++p == pe )
		goto _out134;
case 134:
#line 4571 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr380;
		case 34: goto tr74;
		case 39: goto tr82;
		case 47: goto tr95;
		case 58: goto tr96;
		case 60: goto st100;
		case 61: goto st127;
		case 62: goto tr97;
		case 95: goto tr96;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 13 )
				goto tr381;
		} else if ( (*p) >= 9 )
			goto tr380;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr96;
		} else if ( (*p) >= 65 )
			goto tr96;
	} else
		goto tr94;
	goto st98;
tr235:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st135;
st135:
	if ( ++p == pe )
		goto _out135;
case 135:
#line 4606 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr250;
		case 39: goto tr62;
		case 47: goto tr237;
		case 58: goto tr175;
		case 60: goto tr196;
		case 62: goto tr253;
		case 95: goto tr175;
	}
	if ( (*p) < 45 ) {
		if ( (*p) > 10 ) {
			if ( 11 <= (*p) && (*p) <= 13 )
				goto tr251;
		} else if ( (*p) >= 9 )
			goto tr250;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr175;
		} else if ( (*p) >= 65 )
			goto tr175;
	} else
		goto tr173;
	goto tr166;
st136:
	if ( ++p == pe )
		goto _out136;
case 136:
	switch( (*p) ) {
		case 34: goto tr8;
		case 39: goto tr216;
	}
	goto tr215;
tr170:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st137;
st137:
	if ( ++p == pe )
		goto _out137;
case 137:
#line 4648 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr197;
		case 39: goto tr3;
		case 47: goto tr199;
		case 58: goto tr200;
		case 62: goto tr201;
		case 95: goto tr200;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr197;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr200;
		} else if ( (*p) >= 65 )
			goto tr200;
	} else
		goto tr198;
	goto tr196;
tr179:
#line 57 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); }
	goto st138;
st138:
	if ( ++p == pe )
		goto _out138;
case 138:
#line 4677 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr210;
		case 34: goto tr3;
		case 47: goto tr212;
		case 58: goto tr213;
		case 62: goto tr214;
		case 95: goto tr213;
	}
	if ( (*p) < 45 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr210;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr213;
		} else if ( (*p) >= 65 )
			goto tr213;
	} else
		goto tr211;
	goto tr209;
st139:
	if ( ++p == pe )
		goto _out139;
case 139:
	switch( (*p) ) {
		case 34: goto tr225;
		case 39: goto tr9;
	}
	goto tr215;
st140:
	if ( ++p == pe )
		goto _out140;
case 140:
	switch( (*p) ) {
		case 32: goto tr250;
		case 39: goto tr62;
		case 47: goto tr236;
		case 60: goto tr196;
		case 62: goto tr252;
	}
	if ( (*p) > 10 ) {
		if ( 11 <= (*p) && (*p) <= 13 )
			goto tr251;
	} else if ( (*p) >= 9 )
		goto tr250;
	goto tr166;
st141:
	if ( ++p == pe )
		goto _out141;
case 141:
	if ( (*p) == 34 )
		goto tr3;
	goto tr209;
st142:
	if ( ++p == pe )
		goto _out142;
case 142:
	if ( (*p) == 39 )
		goto tr3;
	goto tr196;
tr325:
#line 55 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(tag, p); }
	goto st143;
st143:
	if ( ++p == pe )
		goto _out143;
case 143:
#line 4746 "ext/hpricot_scan/hpricot_scan.c"
	if ( (*p) == 62 )
		goto tr355;
	goto tr294;
st144:
	if ( ++p == pe )
		goto _out144;
case 144:
	switch( (*p) ) {
		case 58: goto st145;
		case 95: goto st145;
		case 120: goto st146;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st145;
	} else if ( (*p) >= 65 )
		goto st145;
	goto tr294;
st145:
	if ( ++p == pe )
		goto _out145;
case 145:
	switch( (*p) ) {
		case 32: goto st221;
		case 95: goto st145;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st145;
		} else if ( (*p) >= 9 )
			goto st221;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st145;
		} else if ( (*p) >= 65 )
			goto st145;
	} else
		goto st145;
	goto tr294;
st221:
	if ( ++p == pe )
		goto _out221;
case 221:
	if ( (*p) == 32 )
		goto st221;
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st221;
	goto tr11;
st146:
	if ( ++p == pe )
		goto _out146;
case 146:
	switch( (*p) ) {
		case 32: goto st221;
		case 95: goto st145;
		case 109: goto st147;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st145;
		} else if ( (*p) >= 9 )
			goto st221;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st145;
		} else if ( (*p) >= 65 )
			goto st145;
	} else
		goto st145;
	goto tr294;
st147:
	if ( ++p == pe )
		goto _out147;
case 147:
	switch( (*p) ) {
		case 32: goto st221;
		case 95: goto st145;
		case 108: goto st148;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st145;
		} else if ( (*p) >= 9 )
			goto st221;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st145;
		} else if ( (*p) >= 65 )
			goto st145;
	} else
		goto st145;
	goto tr294;
st148:
	if ( ++p == pe )
		goto _out148;
case 148:
	switch( (*p) ) {
		case 32: goto tr13;
		case 95: goto st145;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 45 <= (*p) && (*p) <= 46 )
				goto st145;
		} else if ( (*p) >= 9 )
			goto tr13;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st145;
		} else if ( (*p) >= 65 )
			goto st145;
	} else
		goto st145;
	goto tr294;
tr13:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokend = p+1;}
	goto st222;
st222:
	if ( ++p == pe )
		goto _out222;
case 222:
#line 4876 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto tr13;
		case 118: goto st149;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr13;
	goto tr11;
st149:
	if ( ++p == pe )
		goto _out149;
case 149:
	if ( (*p) == 101 )
		goto st150;
	goto tr262;
st150:
	if ( ++p == pe )
		goto _out150;
case 150:
	if ( (*p) == 114 )
		goto st151;
	goto tr262;
st151:
	if ( ++p == pe )
		goto _out151;
case 151:
	if ( (*p) == 115 )
		goto st152;
	goto tr262;
st152:
	if ( ++p == pe )
		goto _out152;
case 152:
	if ( (*p) == 105 )
		goto st153;
	goto tr262;
st153:
	if ( ++p == pe )
		goto _out153;
case 153:
	if ( (*p) == 111 )
		goto st154;
	goto tr262;
st154:
	if ( ++p == pe )
		goto _out154;
case 154:
	if ( (*p) == 110 )
		goto st155;
	goto tr262;
st155:
	if ( ++p == pe )
		goto _out155;
case 155:
	switch( (*p) ) {
		case 32: goto st155;
		case 61: goto st156;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st155;
	goto tr262;
st156:
	if ( ++p == pe )
		goto _out156;
case 156:
	switch( (*p) ) {
		case 32: goto st156;
		case 34: goto st157;
		case 39: goto st199;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st156;
	goto tr262;
st157:
	if ( ++p == pe )
		goto _out157;
case 157:
	if ( (*p) == 95 )
		goto tr279;
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto tr279;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr279;
		} else if ( (*p) >= 65 )
			goto tr279;
	} else
		goto tr279;
	goto tr262;
tr279:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st158;
st158:
	if ( ++p == pe )
		goto _out158;
case 158:
#line 4975 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 34: goto tr272;
		case 95: goto st158;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st158;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st158;
		} else if ( (*p) >= 65 )
			goto st158;
	} else
		goto st158;
	goto tr262;
tr272:
#line 59 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("version"), aval); }
	goto st159;
st159:
	if ( ++p == pe )
		goto _out159;
case 159:
#line 5000 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st160;
		case 63: goto st161;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st160;
	goto tr262;
st160:
	if ( ++p == pe )
		goto _out160;
case 160:
	switch( (*p) ) {
		case 32: goto st160;
		case 63: goto st161;
		case 101: goto st162;
		case 115: goto st175;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st160;
	goto tr262;
st161:
	if ( ++p == pe )
		goto _out161;
case 161:
	if ( (*p) == 62 )
		goto tr293;
	goto tr262;
st162:
	if ( ++p == pe )
		goto _out162;
case 162:
	if ( (*p) == 110 )
		goto st163;
	goto tr262;
st163:
	if ( ++p == pe )
		goto _out163;
case 163:
	if ( (*p) == 99 )
		goto st164;
	goto tr262;
st164:
	if ( ++p == pe )
		goto _out164;
case 164:
	if ( (*p) == 111 )
		goto st165;
	goto tr262;
st165:
	if ( ++p == pe )
		goto _out165;
case 165:
	if ( (*p) == 100 )
		goto st166;
	goto tr262;
st166:
	if ( ++p == pe )
		goto _out166;
case 166:
	if ( (*p) == 105 )
		goto st167;
	goto tr262;
st167:
	if ( ++p == pe )
		goto _out167;
case 167:
	if ( (*p) == 110 )
		goto st168;
	goto tr262;
st168:
	if ( ++p == pe )
		goto _out168;
case 168:
	if ( (*p) == 103 )
		goto st169;
	goto tr262;
st169:
	if ( ++p == pe )
		goto _out169;
case 169:
	switch( (*p) ) {
		case 32: goto st169;
		case 61: goto st170;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st169;
	goto tr262;
st170:
	if ( ++p == pe )
		goto _out170;
case 170:
	switch( (*p) ) {
		case 32: goto st170;
		case 34: goto st171;
		case 39: goto st197;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st170;
	goto tr262;
st171:
	if ( ++p == pe )
		goto _out171;
case 171:
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr280;
	} else if ( (*p) >= 65 )
		goto tr280;
	goto tr262;
tr280:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st172;
st172:
	if ( ++p == pe )
		goto _out172;
case 172:
#line 5118 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 34: goto tr274;
		case 95: goto st172;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st172;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st172;
		} else if ( (*p) >= 65 )
			goto st172;
	} else
		goto st172;
	goto tr262;
tr274:
#line 60 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("encoding"), aval); }
	goto st173;
st173:
	if ( ++p == pe )
		goto _out173;
case 173:
#line 5143 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st174;
		case 63: goto st161;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st174;
	goto tr262;
st174:
	if ( ++p == pe )
		goto _out174;
case 174:
	switch( (*p) ) {
		case 32: goto st174;
		case 63: goto st161;
		case 115: goto st175;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st174;
	goto tr262;
st175:
	if ( ++p == pe )
		goto _out175;
case 175:
	if ( (*p) == 116 )
		goto st176;
	goto tr262;
st176:
	if ( ++p == pe )
		goto _out176;
case 176:
	if ( (*p) == 97 )
		goto st177;
	goto tr262;
st177:
	if ( ++p == pe )
		goto _out177;
case 177:
	if ( (*p) == 110 )
		goto st178;
	goto tr262;
st178:
	if ( ++p == pe )
		goto _out178;
case 178:
	if ( (*p) == 100 )
		goto st179;
	goto tr262;
st179:
	if ( ++p == pe )
		goto _out179;
case 179:
	if ( (*p) == 97 )
		goto st180;
	goto tr262;
st180:
	if ( ++p == pe )
		goto _out180;
case 180:
	if ( (*p) == 108 )
		goto st181;
	goto tr262;
st181:
	if ( ++p == pe )
		goto _out181;
case 181:
	if ( (*p) == 111 )
		goto st182;
	goto tr262;
st182:
	if ( ++p == pe )
		goto _out182;
case 182:
	if ( (*p) == 110 )
		goto st183;
	goto tr262;
st183:
	if ( ++p == pe )
		goto _out183;
case 183:
	if ( (*p) == 101 )
		goto st184;
	goto tr262;
st184:
	if ( ++p == pe )
		goto _out184;
case 184:
	switch( (*p) ) {
		case 32: goto st184;
		case 61: goto st185;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st184;
	goto tr262;
st185:
	if ( ++p == pe )
		goto _out185;
case 185:
	switch( (*p) ) {
		case 32: goto st185;
		case 34: goto st186;
		case 39: goto st192;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st185;
	goto tr262;
st186:
	if ( ++p == pe )
		goto _out186;
case 186:
	switch( (*p) ) {
		case 110: goto tr288;
		case 121: goto tr289;
	}
	goto tr262;
tr288:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st187;
st187:
	if ( ++p == pe )
		goto _out187;
case 187:
#line 5266 "ext/hpricot_scan/hpricot_scan.c"
	if ( (*p) == 111 )
		goto st188;
	goto tr262;
st188:
	if ( ++p == pe )
		goto _out188;
case 188:
	if ( (*p) == 34 )
		goto tr276;
	goto tr262;
tr276:
#line 61 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(aval, p); ATTR(rb_str_new2("standalone"), aval); }
	goto st189;
st189:
	if ( ++p == pe )
		goto _out189;
case 189:
#line 5285 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 32: goto st189;
		case 63: goto st161;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st189;
	goto tr262;
tr289:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st190;
st190:
	if ( ++p == pe )
		goto _out190;
case 190:
#line 5301 "ext/hpricot_scan/hpricot_scan.c"
	if ( (*p) == 101 )
		goto st191;
	goto tr262;
st191:
	if ( ++p == pe )
		goto _out191;
case 191:
	if ( (*p) == 115 )
		goto st188;
	goto tr262;
st192:
	if ( ++p == pe )
		goto _out192;
case 192:
	switch( (*p) ) {
		case 110: goto tr414;
		case 121: goto tr415;
	}
	goto tr262;
tr414:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st193;
st193:
	if ( ++p == pe )
		goto _out193;
case 193:
#line 5329 "ext/hpricot_scan/hpricot_scan.c"
	if ( (*p) == 111 )
		goto st194;
	goto tr262;
st194:
	if ( ++p == pe )
		goto _out194;
case 194:
	if ( (*p) == 39 )
		goto tr276;
	goto tr262;
tr415:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st195;
st195:
	if ( ++p == pe )
		goto _out195;
case 195:
#line 5348 "ext/hpricot_scan/hpricot_scan.c"
	if ( (*p) == 101 )
		goto st196;
	goto tr262;
st196:
	if ( ++p == pe )
		goto _out196;
case 196:
	if ( (*p) == 115 )
		goto st194;
	goto tr262;
st197:
	if ( ++p == pe )
		goto _out197;
case 197:
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr392;
	} else if ( (*p) >= 65 )
		goto tr392;
	goto tr262;
tr392:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st198;
st198:
	if ( ++p == pe )
		goto _out198;
case 198:
#line 5377 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 39: goto tr274;
		case 95: goto st198;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st198;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st198;
		} else if ( (*p) >= 65 )
			goto st198;
	} else
		goto st198;
	goto tr262;
st199:
	if ( ++p == pe )
		goto _out199;
case 199:
	if ( (*p) == 95 )
		goto tr391;
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto tr391;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr391;
		} else if ( (*p) >= 65 )
			goto tr391;
	} else
		goto tr391;
	goto tr262;
tr391:
#line 53 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_aval = p; }
	goto st200;
st200:
	if ( ++p == pe )
		goto _out200;
case 200:
#line 5420 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 39: goto tr272;
		case 95: goto st200;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st200;
	} else if ( (*p) > 58 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st200;
		} else if ( (*p) >= 65 )
			goto st200;
	} else
		goto st200;
	goto tr262;
st201:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokstart = 0;}
	if ( ++p == pe )
		goto _out201;
case 201:
#line 5443 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 10: goto tr425;
		case 45: goto tr426;
	}
	goto tr424;
tr424:
#line 52 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_tag = p; }
	goto st202;
tr420:
#line 80 "ext/hpricot_scan/hpricot_scan.rl"
	{curline += 1;}
	goto st202;
tr425:
#line 52 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_tag = p; }
#line 80 "ext/hpricot_scan/hpricot_scan.rl"
	{curline += 1;}
	goto st202;
st202:
	if ( ++p == pe )
		goto _out202;
case 202:
#line 5467 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 10: goto tr420;
		case 45: goto tr421;
	}
	goto st202;
tr421:
#line 56 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(tag, p-1); }
	goto st203;
tr426:
#line 52 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_tag = p; }
#line 56 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(tag, p-1); }
	goto st203;
st203:
	if ( ++p == pe )
		goto _out203;
case 203:
#line 5487 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 10: goto tr420;
		case 45: goto tr422;
	}
	goto st202;
tr422:
#line 56 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(tag, p-1); }
	goto st204;
st204:
	if ( ++p == pe )
		goto _out204;
case 204:
#line 5501 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 10: goto tr420;
		case 45: goto tr422;
		case 62: goto tr423;
	}
	goto st202;
tr423:
#line 119 "ext/hpricot_scan/hpricot_scan.rl"
	{ ELE(comment); {goto st213;} }
	goto st223;
st223:
	if ( ++p == pe )
		goto _out223;
case 223:
#line 5516 "ext/hpricot_scan/hpricot_scan.c"
	goto st205;
st205:
	goto _out205;
st206:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokstart = 0;}
	if ( ++p == pe )
		goto _out206;
case 206:
#line 5526 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 10: goto tr433;
		case 93: goto tr434;
	}
	goto tr432;
tr432:
#line 52 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_tag = p; }
	goto st207;
tr428:
#line 80 "ext/hpricot_scan/hpricot_scan.rl"
	{curline += 1;}
	goto st207;
tr433:
#line 52 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_tag = p; }
#line 80 "ext/hpricot_scan/hpricot_scan.rl"
	{curline += 1;}
	goto st207;
st207:
	if ( ++p == pe )
		goto _out207;
case 207:
#line 5550 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 10: goto tr428;
		case 93: goto tr429;
	}
	goto st207;
tr429:
#line 56 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(tag, p-1); }
	goto st208;
tr434:
#line 52 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_tag = p; }
#line 56 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(tag, p-1); }
	goto st208;
st208:
	if ( ++p == pe )
		goto _out208;
case 208:
#line 5570 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 10: goto tr428;
		case 93: goto tr431;
	}
	goto st207;
tr431:
#line 56 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(tag, p-1); }
	goto st209;
st209:
	if ( ++p == pe )
		goto _out209;
case 209:
#line 5584 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 10: goto tr428;
		case 62: goto tr430;
		case 93: goto tr431;
	}
	goto st207;
tr430:
#line 121 "ext/hpricot_scan/hpricot_scan.rl"
	{ ELE(cdata); {goto st213;} }
	goto st224;
st224:
	if ( ++p == pe )
		goto _out224;
case 224:
#line 5599 "ext/hpricot_scan/hpricot_scan.c"
	goto st205;
st210:
#line 1 "ext/hpricot_scan/hpricot_scan.rl"
	{tokstart = 0;}
	if ( ++p == pe )
		goto _out210;
case 210:
#line 5607 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 10: goto tr440;
		case 63: goto tr441;
	}
	goto tr439;
tr439:
#line 52 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_tag = p; }
	goto st211;
tr436:
#line 80 "ext/hpricot_scan/hpricot_scan.rl"
	{curline += 1;}
	goto st211;
tr440:
#line 52 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_tag = p; }
#line 80 "ext/hpricot_scan/hpricot_scan.rl"
	{curline += 1;}
	goto st211;
st211:
	if ( ++p == pe )
		goto _out211;
case 211:
#line 5631 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 10: goto tr436;
		case 63: goto tr437;
	}
	goto st211;
tr437:
#line 56 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(tag, p-1); }
	goto st212;
tr441:
#line 52 "ext/hpricot_scan/hpricot_scan.rl"
	{ mark_tag = p; }
#line 56 "ext/hpricot_scan/hpricot_scan.rl"
	{ SET(tag, p-1); }
	goto st212;
st212:
	if ( ++p == pe )
		goto _out212;
case 212:
#line 5651 "ext/hpricot_scan/hpricot_scan.c"
	switch( (*p) ) {
		case 10: goto tr436;
		case 62: goto tr438;
		case 63: goto tr437;
	}
	goto st211;
tr438:
#line 123 "ext/hpricot_scan/hpricot_scan.rl"
	{ ELE(procins); {goto st213;} }
	goto st225;
st225:
	if ( ++p == pe )
		goto _out225;
case 225:
#line 5666 "ext/hpricot_scan/hpricot_scan.c"
	goto st205;
	}
	_out213: cs = 213; goto _out; 
	_out214: cs = 214; goto _out; 
	_out0: cs = 0; goto _out; 
	_out1: cs = 1; goto _out; 
	_out2: cs = 2; goto _out; 
	_out3: cs = 3; goto _out; 
	_out4: cs = 4; goto _out; 
	_out5: cs = 5; goto _out; 
	_out6: cs = 6; goto _out; 
	_out7: cs = 7; goto _out; 
	_out8: cs = 8; goto _out; 
	_out9: cs = 9; goto _out; 
	_out10: cs = 10; goto _out; 
	_out11: cs = 11; goto _out; 
	_out12: cs = 12; goto _out; 
	_out13: cs = 13; goto _out; 
	_out14: cs = 14; goto _out; 
	_out15: cs = 15; goto _out; 
	_out16: cs = 16; goto _out; 
	_out17: cs = 17; goto _out; 
	_out18: cs = 18; goto _out; 
	_out19: cs = 19; goto _out; 
	_out20: cs = 20; goto _out; 
	_out21: cs = 21; goto _out; 
	_out22: cs = 22; goto _out; 
	_out23: cs = 23; goto _out; 
	_out24: cs = 24; goto _out; 
	_out25: cs = 25; goto _out; 
	_out26: cs = 26; goto _out; 
	_out27: cs = 27; goto _out; 
	_out28: cs = 28; goto _out; 
	_out29: cs = 29; goto _out; 
	_out30: cs = 30; goto _out; 
	_out31: cs = 31; goto _out; 
	_out32: cs = 32; goto _out; 
	_out33: cs = 33; goto _out; 
	_out34: cs = 34; goto _out; 
	_out35: cs = 35; goto _out; 
	_out36: cs = 36; goto _out; 
	_out37: cs = 37; goto _out; 
	_out38: cs = 38; goto _out; 
	_out39: cs = 39; goto _out; 
	_out215: cs = 215; goto _out; 
	_out40: cs = 40; goto _out; 
	_out41: cs = 41; goto _out; 
	_out216: cs = 216; goto _out; 
	_out42: cs = 42; goto _out; 
	_out43: cs = 43; goto _out; 
	_out217: cs = 217; goto _out; 
	_out44: cs = 44; goto _out; 
	_out45: cs = 45; goto _out; 
	_out46: cs = 46; goto _out; 
	_out47: cs = 47; goto _out; 
	_out48: cs = 48; goto _out; 
	_out49: cs = 49; goto _out; 
	_out50: cs = 50; goto _out; 
	_out51: cs = 51; goto _out; 
	_out52: cs = 52; goto _out; 
	_out53: cs = 53; goto _out; 
	_out54: cs = 54; goto _out; 
	_out55: cs = 55; goto _out; 
	_out56: cs = 56; goto _out; 
	_out57: cs = 57; goto _out; 
	_out58: cs = 58; goto _out; 
	_out59: cs = 59; goto _out; 
	_out60: cs = 60; goto _out; 
	_out61: cs = 61; goto _out; 
	_out62: cs = 62; goto _out; 
	_out63: cs = 63; goto _out; 
	_out64: cs = 64; goto _out; 
	_out65: cs = 65; goto _out; 
	_out66: cs = 66; goto _out; 
	_out67: cs = 67; goto _out; 
	_out68: cs = 68; goto _out; 
	_out69: cs = 69; goto _out; 
	_out70: cs = 70; goto _out; 
	_out71: cs = 71; goto _out; 
	_out72: cs = 72; goto _out; 
	_out73: cs = 73; goto _out; 
	_out74: cs = 74; goto _out; 
	_out75: cs = 75; goto _out; 
	_out76: cs = 76; goto _out; 
	_out77: cs = 77; goto _out; 
	_out78: cs = 78; goto _out; 
	_out79: cs = 79; goto _out; 
	_out80: cs = 80; goto _out; 
	_out81: cs = 81; goto _out; 
	_out82: cs = 82; goto _out; 
	_out83: cs = 83; goto _out; 
	_out84: cs = 84; goto _out; 
	_out218: cs = 218; goto _out; 
	_out85: cs = 85; goto _out; 
	_out86: cs = 86; goto _out; 
	_out87: cs = 87; goto _out; 
	_out88: cs = 88; goto _out; 
	_out89: cs = 89; goto _out; 
	_out90: cs = 90; goto _out; 
	_out91: cs = 91; goto _out; 
	_out92: cs = 92; goto _out; 
	_out93: cs = 93; goto _out; 
	_out94: cs = 94; goto _out; 
	_out95: cs = 95; goto _out; 
	_out96: cs = 96; goto _out; 
	_out97: cs = 97; goto _out; 
	_out98: cs = 98; goto _out; 
	_out99: cs = 99; goto _out; 
	_out100: cs = 100; goto _out; 
	_out101: cs = 101; goto _out; 
	_out102: cs = 102; goto _out; 
	_out103: cs = 103; goto _out; 
	_out104: cs = 104; goto _out; 
	_out219: cs = 219; goto _out; 
	_out105: cs = 105; goto _out; 
	_out106: cs = 106; goto _out; 
	_out107: cs = 107; goto _out; 
	_out108: cs = 108; goto _out; 
	_out109: cs = 109; goto _out; 
	_out110: cs = 110; goto _out; 
	_out111: cs = 111; goto _out; 
	_out112: cs = 112; goto _out; 
	_out113: cs = 113; goto _out; 
	_out114: cs = 114; goto _out; 
	_out115: cs = 115; goto _out; 
	_out116: cs = 116; goto _out; 
	_out117: cs = 117; goto _out; 
	_out118: cs = 118; goto _out; 
	_out119: cs = 119; goto _out; 
	_out120: cs = 120; goto _out; 
	_out220: cs = 220; goto _out; 
	_out121: cs = 121; goto _out; 
	_out122: cs = 122; goto _out; 
	_out123: cs = 123; goto _out; 
	_out124: cs = 124; goto _out; 
	_out125: cs = 125; goto _out; 
	_out126: cs = 126; goto _out; 
	_out127: cs = 127; goto _out; 
	_out128: cs = 128; goto _out; 
	_out129: cs = 129; goto _out; 
	_out130: cs = 130; goto _out; 
	_out131: cs = 131; goto _out; 
	_out132: cs = 132; goto _out; 
	_out133: cs = 133; goto _out; 
	_out134: cs = 134; goto _out; 
	_out135: cs = 135; goto _out; 
	_out136: cs = 136; goto _out; 
	_out137: cs = 137; goto _out; 
	_out138: cs = 138; goto _out; 
	_out139: cs = 139; goto _out; 
	_out140: cs = 140; goto _out; 
	_out141: cs = 141; goto _out; 
	_out142: cs = 142; goto _out; 
	_out143: cs = 143; goto _out; 
	_out144: cs = 144; goto _out; 
	_out145: cs = 145; goto _out; 
	_out221: cs = 221; goto _out; 
	_out146: cs = 146; goto _out; 
	_out147: cs = 147; goto _out; 
	_out148: cs = 148; goto _out; 
	_out222: cs = 222; goto _out; 
	_out149: cs = 149; goto _out; 
	_out150: cs = 150; goto _out; 
	_out151: cs = 151; goto _out; 
	_out152: cs = 152; goto _out; 
	_out153: cs = 153; goto _out; 
	_out154: cs = 154; goto _out; 
	_out155: cs = 155; goto _out; 
	_out156: cs = 156; goto _out; 
	_out157: cs = 157; goto _out; 
	_out158: cs = 158; goto _out; 
	_out159: cs = 159; goto _out; 
	_out160: cs = 160; goto _out; 
	_out161: cs = 161; goto _out; 
	_out162: cs = 162; goto _out; 
	_out163: cs = 163; goto _out; 
	_out164: cs = 164; goto _out; 
	_out165: cs = 165; goto _out; 
	_out166: cs = 166; goto _out; 
	_out167: cs = 167; goto _out; 
	_out168: cs = 168; goto _out; 
	_out169: cs = 169; goto _out; 
	_out170: cs = 170; goto _out; 
	_out171: cs = 171; goto _out; 
	_out172: cs = 172; goto _out; 
	_out173: cs = 173; goto _out; 
	_out174: cs = 174; goto _out; 
	_out175: cs = 175; goto _out; 
	_out176: cs = 176; goto _out; 
	_out177: cs = 177; goto _out; 
	_out178: cs = 178; goto _out; 
	_out179: cs = 179; goto _out; 
	_out180: cs = 180; goto _out; 
	_out181: cs = 181; goto _out; 
	_out182: cs = 182; goto _out; 
	_out183: cs = 183; goto _out; 
	_out184: cs = 184; goto _out; 
	_out185: cs = 185; goto _out; 
	_out186: cs = 186; goto _out; 
	_out187: cs = 187; goto _out; 
	_out188: cs = 188; goto _out; 
	_out189: cs = 189; goto _out; 
	_out190: cs = 190; goto _out; 
	_out191: cs = 191; goto _out; 
	_out192: cs = 192; goto _out; 
	_out193: cs = 193; goto _out; 
	_out194: cs = 194; goto _out; 
	_out195: cs = 195; goto _out; 
	_out196: cs = 196; goto _out; 
	_out197: cs = 197; goto _out; 
	_out198: cs = 198; goto _out; 
	_out199: cs = 199; goto _out; 
	_out200: cs = 200; goto _out; 
	_out201: cs = 201; goto _out; 
	_out202: cs = 202; goto _out; 
	_out203: cs = 203; goto _out; 
	_out204: cs = 204; goto _out; 
	_out223: cs = 223; goto _out; 
	_out205: cs = 205; goto _out; 
	_out206: cs = 206; goto _out; 
	_out207: cs = 207; goto _out; 
	_out208: cs = 208; goto _out; 
	_out209: cs = 209; goto _out; 
	_out224: cs = 224; goto _out; 
	_out210: cs = 210; goto _out; 
	_out211: cs = 211; goto _out; 
	_out212: cs = 212; goto _out; 
	_out225: cs = 225; goto _out; 

	_out: {}
	}
#line 234 "ext/hpricot_scan/hpricot_scan.rl"
    
    if ( cs == hpricot_scan_error ) {
      fprintf(stderr, "PARSE ERROR\n" );
      break;
    }
    
    if ( done && ele_open )
    {
      ele_open = 0;
      if (tokstart > 0) {
        mark_tag = tokstart;
        tokstart = 0;
        text = 1;
      }
    }

    if ( tokstart == 0 )
    {
      have = 0;
      /* text nodes have no tokstart because each byte is parsed alone */
      if ( mark_tag != NULL && text == 1 )
      {
        if (done)
        {
          if (mark_tag < p-1)
          {
            CAT(tag, p-1);
            ELE(text);
          }
        }
        else
        {
          CAT(tag, p);
        }
      }
      mark_tag = buf;
    }
    else
    {
      have = pe - tokstart;
      memmove( buf, tokstart, have );
      SLIDE(tag);
      SLIDE(akey);
      SLIDE(aval);
      tokend = buf + (tokend - tokstart);
      tokstart = buf;
    }
  }
}

void Init_hpricot_scan()
{
  VALUE mHpricot = rb_define_module("Hpricot");
  rb_define_singleton_method(mHpricot, "scan", hpricot_scan, 1);

  s_read = rb_intern("read");
  s_to_str = rb_intern("to_str");
  sym_xmldecl = ID2SYM(rb_intern("xmldecl"));
  sym_doctype = ID2SYM(rb_intern("doctype"));
  sym_procins = ID2SYM(rb_intern("procins"));
  sym_stag = ID2SYM(rb_intern("stag"));
  sym_etag = ID2SYM(rb_intern("etag"));
  sym_emptytag = ID2SYM(rb_intern("emptytag"));
  sym_comment = ID2SYM(rb_intern("comment"));
  sym_cdata = ID2SYM(rb_intern("cdata"));
  sym_text = ID2SYM(rb_intern("text"));
}
