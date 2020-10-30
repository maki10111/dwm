/* See LICENSE file for copyright and license details. */
/* Constants */
#define TERMINAL "st"
#define TERMCLASS "St"

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "MesloLGS NF:size=10", "JoyPixels:pixelsize=12:antialias=true:autohint=true"  };
static char dmenufont[]             = "MesloLGS NF:size=10";
static char normbgcolor[]           = "#000000";
static char normbordercolor[]       = "#00fa9a";
static char normfgcolor[]           = "#eeeeee";
static char selfgcolor[]            = "#1e90ff";
static char selbordercolor[]        = "#00fa9a";
static char selbgcolor[]            = "#000000";
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;

static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {TERMINAL, "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {TERMINAL, "-f", "monospace:size=16", "-g", "50x20", "-e", "bc", "-lq", NULL };

static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spranger",    spcmd2},

};

/* tagging */
//uses  as tags
static const char *tags[] = { "\uf292", "\uf292", "\uf292", "\uf292", "\uf292", "\uf292", "\uf292", "\uf292", "\uf292" };
//uses  笠as tags
//static const char *tags[] = { "\uf9f8", "\uf9f8", "\uf9f8", "\uf9f8", "\uf9f8", "\uf9f8", "\uf9f8", "\uf9f8", "\uf9f8" };
//uses  as tags
//static const char *tags[] = { "\uf673", "\uf673", "\uf673", "\uf673", "\uf673", "\uf673", "\uf673", "\uf673", "\uf673" };

//uses numbers like  as alt tags
static const char *tagsalt[] ={ "\uf8a3", "\uf8a6", "\uf8a9", "\uf8ac", "\uf8af", "\uf8b2", "\uf8b5", "\uf8b8", "\uf8bb" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	*/
	/* class    instance      title       	 tags mask    isfloating   isterminal  noswallow  monitor */

	{ "Gimp",     NULL,       NULL,       	    1 << 8,       0,           0,         0,        -1 },
	{ TERMCLASS,   NULL,       NULL,       	    0,            0,           1,         0,        -1 },
	{ NULL,       NULL,       "Event Tester",   0,            0,           0,         1,        -1 },
	{ NULL,      "spterm",    NULL,       	    SPTAG(0),     1,           1,         0,        -1 },



};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

/* mouse scroll resize */
static const int scrollsensetivity = 30; /* 1 means resize window by 1 pixel for each scroll event */
#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "\uf8a3",	tile },			/* 1 Default: Master on left, slaves on right */
	{ "\uf8a6",	bstack },		/* 2 Master on top, slaves on bottom */
	{ "\uf8a9",	spiral },		/* 3 Fibonacci spiral */
	{ "\uf8ac",	dwindle },		/* 4 Decreasing in size right a nd leftward */
	{ "\uf8af",	deck },			/* 5 Master on left, slaves in monocle-like mode on right */
 	{ "\uf8b2",	monocle },		/* 6 All windows on top of eachother */
	{ "\uf8b5",	centeredmaster },	/* 7 Master in middle, slaves on sides */
	{ "\uf8b8",	centeredfloatingmaster },/*8 Same but master floats */
	{ "\uf8bb",	NULL },			/*9 no layout function means floating behavior */
	{ NULL,		NULL },

};



/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD,	XK_j,	ACTION##stack,	{.i = INC(+1) } }, \
	{ MOD,	XK_k,	ACTION##stack,	{.i = INC(-1) } }, \
	{ MOD,  XK_v,   ACTION##stack,  {.i = 0 } }, \

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { TERMINAL, NULL };
static const char *browser[]  = { "brave", NULL };
static const char *code[]  = { "code", NULL };

static const char *libreoffice[]  = { "libreoffice", NULL };

#include <X11/XF86keysym.h>
#include "shiftview.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)
	{ MODKEY,				XK_d,		spawn,	SHCMD("dmenuunicode") },
	{ MODKEY,			XK_F4,		spawn,	SHCMD("sysact") },

	TAGKEYS(			XK_1,		0)
	TAGKEYS(			XK_2,		1)
	TAGKEYS(			XK_3,		2)
	TAGKEYS(			XK_4,		3)
	TAGKEYS(			XK_5,		4)
	TAGKEYS(			XK_6,		5)
	TAGKEYS(			XK_7,		6)
	TAGKEYS(			XK_8,		7)
	TAGKEYS(			XK_9,		8)



	{ MODKEY,				XK_0,		view,		{.ui = ~0 } },
	{ MODKEY|ShiftMask,		XK_0,		tag,		{.ui = ~0 } },
	{ MODKEY,				XK_w,		spawn,		{.v = browser }},
	{ MODKEY,				XK_v,		spawn,		{.v = code} },
	{ MODKEY,				XK_e,		spawn,		SHCMD(TERMINAL " -e lf ~/") },
	{ MODKEY,				XK_o,		spawn,		{.v = libreoffice} },
	{ MODKEY,				XK_g,		spawn,		SHCMD(TERMINAL " -e games") },
	{MODKEY,				XK_q,		killclient,	{0} },


	{ MODKEY,				XK_n,		spawn,		SHCMD(TERMINAL " -e sudo nmtui") },
//LAYOUTS

	{ MODKEY,				XK_t,		setlayout,	{.v = &layouts[0]} }, /* tile */
	{ MODKEY|ShiftMask,			XK_t,		setlayout,	{.v = &layouts[1]} }, /* bstack */
	{ MODKEY,				XK_y,		setlayout,	{.v = &layouts[2]} }, /* spiral */
	{ MODKEY|ShiftMask,			XK_y,		setlayout,	{.v = &layouts[3]} }, /* dwindle */
	{ MODKEY,				XK_u,		setlayout,	{.v = &layouts[4]} }, /* deck */
	{ MODKEY|ShiftMask,			XK_u,		setlayout,	{.v = &layouts[5]} }, /* monocle */
	{ MODKEY,				XK_i,		setlayout,	{.v = &layouts[6]} }, /* centeredmaster */
	{ MODKEY|ShiftMask,			XK_i,		setlayout,	{.v = &layouts[7]} }, /* centeredfloatingmaster */
	{ MODKEY,				XK_bracketleft,		incnmaster, {.i = +1 } }, //Increase the Master Windows
	{ MODKEY,				XK_bracketright,	incnmaster, {.i = -1 } }, //Decrease the Master Windows
	{ MODKEY,				XK_semicolon,		incrgaps, {.i = +3 } }, //Increase the Master Windows
	{ MODKEY,				XK_apostrophe,		incrgaps, {.i = -3 } }, //Decrease the Master Windows


	{ MODKEY,				XK_backslash,		view,		{0} },

	{ MODKEY,				XK_a,		togglegaps,		{0} },
	{ MODKEY,				XK_s,		togglesticky,	{0} },
	{ MODKEY,				XK_r,		spawn,          {.v = dmenucmd } },
	{ MODKEY,				XK_f,		togglefullscr,	{0} },
	{ MODKEY|ShiftMask,				XK_Left,		setmfact,	{.f = -0.025} },
	/* J and K are automatically bound above in STACKEYS */
	{ MODKEY|ShiftMask,				XK_Right,			setmfact,      	{.f = +0.025} },
	{ MODKEY,				XK_x,	spawn,		{.v = termcmd } },

	{ MODKEY,				XK_z,		spawn,	SHCMD(TERMINAL " -e watchv") },

	 { MODKEY,			XK_c,		spawn,		SHCMD(TERMINAL " -e camtoggle") },
	{ MODKEY,			XK_b,		spawn,		SHCMD(TERMINAL " -e btcon") },
	{ MODKEY|ShiftMask,		XK_b,		spawn,		SHCMD("btdcon") },


	{ MODKEY,			XK_m,		spawn,		SHCMD(TERMINAL " -e ncmpcpp") },
	{ MODKEY,			XK_h,		togglealttag,	{0} },


	{ MODKEY,			XK_Left,		shiftview,	{ .i = -1 } },
	{ MODKEY,			XK_Right,		shiftview,	{ .i = +1 } },

	{ MODKEY,			XK_F3,		spawn,		SHCMD("watchv")},
	{ MODKEY,			XK_F1,		spawn,		SHCMD("dmenumount") },
	{ MODKEY,			XK_F2,		spawn,		SHCMD("dmenuumount") },
	{ MODKEY,			XK_Return,	zoom,		{0} },		 //Master
	{ MODKEY|ShiftMask,		XK_Return,	togglefloating,	{0} }, //Float window
	{ 0,				XK_Print,	spawn,		SHCMD("flameshot full -p ~/pics/screenshits") },
	{ MODKEY,			XK_p,		spawn,		SHCMD("flameshot gui -p ~/pics/screenshits") },

	{ MODKEY,			XK_Print,	spawn,		SHCMD("dmenurecord") },
	{MODKEY|ShiftMask,		XK_Print,	spawn,		SHCMD("dmenurecord kill")},
	{MODKEY, XK_Delete, spawn, SHCMD("dmenurecord kill")},
	{ MODKEY,			XK_Scroll_Lock,	spawn,		SHCMD("killall screenkey || screenkey &") },
	{ 0, XF86XK_AudioMute,		spawn,		SHCMD("pamixer -t; pkill -RTMIN+3 dwmblocks") },
	{ 0, XF86XK_AudioRaiseVolume,	spawn,		SHCMD("pamixer --allow-boost -i 5; pkill -RTMIN+3 dwmblocks") },
	{ 0, XF86XK_AudioLowerVolume,	spawn,		SHCMD("pamixer --allow-boost -d 5; pkill -RTMIN+3 dwmblocks") },
	{ 0, XF86XK_AudioPrev,		spawn,		SHCMD("mpc prev  ") },
	{ 0, XF86XK_AudioNext,		spawn,		SHCMD("mpc next ") },
	{ 0, XF86XK_AudioPlay,		spawn,		SHCMD("mpc toggle ") },
	{ 0, XF86XK_AudioStop,		spawn,		SHCMD("mpc stop ") },
	{ MODKEY|ShiftMask, 		XK_m,		spawn,		SHCMD(TERMINAL " -e neomutt") },
	{ 0, XF86XK_TouchpadOff,	spawn,		SHCMD("synclient TouchpadOff=1") },
	{ 0, XF86XK_TouchpadOn,		spawn,		SHCMD("synclient TouchpadOff=0") },
	{ 0, XF86XK_MonBrightnessUp,	spawn,		SHCMD("sudo xbacklight -inc 10") },
	{ 0, XF86XK_MonBrightnessDown,	spawn,		SHCMD("sudo xbacklight -dec 10") },

	{ MODKEY|ShiftMask,             XK_q,      quit,     {0} },
	/* { MODKEY|Mod4Mask,              XK_h,      incrgaps,       {.i = +1 } }, */
	/* { MODKEY|Mod4Mask,              XK_l,      incrgaps,       {.i = -1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_h,      incrogaps,      {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_l,      incrogaps,      {.i = -1 } }, */
	/* { MODKEY|Mod4Mask|ControlMask,  XK_h,      incrigaps,      {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ControlMask,  XK_l,      incrigaps,      {.i = -1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_0,      defaultgaps,    {0} }, */
	/* { MODKEY,                       XK_y,      incrihgaps,     {.i = +1 } }, */
	/* { MODKEY,                       XK_o,      incrihgaps,     {.i = -1 } }, */
	/* { MODKEY|ControlMask,           XK_y,      incrivgaps,     {.i = +1 } }, */
	/* { MODKEY|ControlMask,           XK_o,      incrivgaps,     {.i = -1 } }, */
	/* { MODKEY|Mod4Mask,              XK_y,      incrohgaps,     {.i = +1 } }, */
	/* { MODKEY|Mod4Mask,              XK_o,      incrohgaps,     {.i = -1 } }, */
	/* { MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } }, */
	/* { MODKEY|ShiftMask,             XK_o,      incrovgaps,     {.i = -1 } }, */




};

/* resizemousescroll direction argument list */
static const int scrollargs[][2] = {
	/* width change         height change */
	{ +scrollsensetivity,	0 },
	{ -scrollsensetivity,	0 },
	{ 0, 				  	+scrollsensetivity },
	{ 0, 					-scrollsensetivity },
};
/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
#ifndef __OpenBSD__
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} },
	{ ClkStatusText,        ShiftMask,      Button1,        sigdwmblocks,   {.i = 6} },
#endif
	{ ClkStatusText,        ShiftMask,      Button3,        spawn,          SHCMD(TERMINAL " -e nvim ~/git/repos/dwmblocks/config.h") },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        defaultgaps,	{0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
//	{ ClkClientWin,		MODKEY,		Button4,	incrgaps,	{.i = +1} },
//	{ ClkClientWin,		MODKEY,		Button5,	incrgaps,	{.i = -1} },

	{ ClkClientWin,         MODKEY,         Button4,        resizemousescroll, {.v = &scrollargs[0]} },
	{ ClkClientWin,         MODKEY,         Button5,        resizemousescroll, {.v = &scrollargs[1]} },
	{ ClkClientWin,         MODKEY,         Button6,        resizemousescroll, {.v = &scrollargs[2]} },
	{ ClkClientWin,         MODKEY,         Button7,        resizemousescroll, {.v = &scrollargs[3]} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkTagBar,		0,		Button4,	shiftview,	{.i = -1} },
	{ ClkTagBar,		0,		Button5,	shiftview,	{.i = 1} },
	{ ClkRootWin,		0,		Button2,	togglebar,	{0} },
};
