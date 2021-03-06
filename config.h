/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx  = 2;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static const int focusonwheel       = 0;
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "", "", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                instance    title           tags mask   isfloating   monitor */
	{ "firefox",            NULL,       NULL,           1 << 0,     0,           -1 },
	{ "RStudio",            NULL,       NULL,           1 << 1,     0,           -1 },
	{ "TelegramDesktop",    NULL,       NULL,           1 << 8,     0,           -1 },
	{ "Display",            NULL,       NULL,           0,          1,           -1 },
	{ "Sxiv",               NULL,       NULL,           0,          1,           -1 },
	{ "mpv",                NULL,       NULL,           0,          1,           -1 },
	{ "R_x11",              NULL,       NULL,           0,          1,           -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */ /* set it to 0 if you want st to fit entirelly on the screen */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "TTT",      bstack },
	{ "|M|",      centeredmaster },
	{ "[\\]",     dwindle },
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "H[]",      deck },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } },
    /*
     * I don't like these keybindings
	 * { MOD, XK_q,     ACTION##stack, {.i = 0 } }, \
	 * { MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \
	 * { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \
	 * { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \
	 * { MOD, XK_x,     ACTION##stack, {.i = -1 } },
     */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "normbgcolor",        STRING,  &normbgcolor },
		{ "normbordercolor",    STRING,  &normbordercolor },
		{ "normfgcolor",        STRING,  &normfgcolor },
		{ "selbgcolor",         STRING,  &selbgcolor },
		{ "selbordercolor",     STRING,  &selbordercolor },
		{ "selfgcolor",         STRING,  &selfgcolor },
		{ "borderpx",          	INTEGER, &borderpx },
		{ "snap",          		INTEGER, &snap },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	INTEGER, &topbar },
		{ "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",       	INTEGER, &resizehints },
		{ "mfact",      	 	FLOAT,   &mfact },
};


#include "shiftviewclients.c"

static Key keys[] = {
	/* modifier                     key         function                argument    */

    /* spawn                                                                         */
	{ MODKEY,                       XK_d,       spawn,                  {.v = dmenucmd } },
	{ MODKEY,                       XK_Return,  spawn,                  {.v = termcmd } },

    /* navigation                                                                    */
	STACKKEYS(MODKEY,                           focus)
	STACKKEYS(MODKEY|ShiftMask,                 push)
	{ MODKEY          ,             XK_q,       killclient,             {0} },
	{ MODKEY,                       XK_0,       view,                   {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,       tag,                    {.ui = ~0 } },
    { MODKEY,                       XK_Tab,     shiftviewclients,       { .i = +1 } },
    { MODKEY|ShiftMask,             XK_Tab,     shiftviewclients,       { .i = -1 } },

    /* layout                                                                       */
	{ MODKEY,                       XK_h,       setmfact,               {.f = -0.05} },
	{ MODKEY,                       XK_l,       setmfact,               {.f = +0.05} },
	{ MODKEY,                       XK_b,       togglebar,              {0} },
	{ MODKEY,                       XK_r,       resetlayout,            {0} },
	{ MODKEY,                       XK_space,   zoom,                   {0} }, /* maybe I will omit this line because of stack patch */
	{ MODKEY          ,             XK_f,       togglefullscr,          {0} },

	{ MODKEY,                       XK_F1,      spawn,                  SHCMD("mansplain") },
	{ MODKEY,                       XK_F2,      spawn,                  SHCMD("torrent") },

	{ MODKEY,                       XK_F5,      setlayout,              {.v = &layouts[0]} },
	{ MODKEY,                       XK_F6,      setlayout,              {.v = &layouts[1]} },
	{ MODKEY,                       XK_F7,      setlayout,              {.v = &layouts[2]} },
	{ MODKEY,                       XK_F8,      setlayout,              {.v = &layouts[3]} },

    /* utilities                                                                    */
	{ MODKEY,                       XK_F9,      spawn,                  SHCMD("mountdrive") },
	{ MODKEY|ShiftMask,             XK_F9,      spawn,                  SHCMD("unmountdrive") },
	{ MODKEY,                       XK_F10,     spawn,                  SHCMD("sysupdate") },
	{ MODKEY,                       XK_F12,     spawn,                  SHCMD("power") },
	{ MODKEY|ShiftMask,             XK_F12,     spawn,                  SHCMD("killprocess") },
	{ MODKEY,                       XK_Print,   spawn,                  SHCMD("screenshot") },
	{ MODKEY|ShiftMask,             XK_Print,   spawn,                  SHCMD("recordscreen") },
	{ MODKEY,                       XK_equal,   spawn,                  SHCMD("pactl set-sink-volume 0 +5% && pkill -RTMIN+10 dwmblocks") },
	{ MODKEY,                       XK_minus,   spawn,                  SHCMD("pactl set-sink-volume 0 -5% && pkill -RTMIN+10 dwmblocks") },

	TAGKEYS(                        XK_1,                               0)
	TAGKEYS(                        XK_2,                               1)
	TAGKEYS(                        XK_3,                               2)
	TAGKEYS(                        XK_4,                               3)
	TAGKEYS(                        XK_5,                               4)
	TAGKEYS(                        XK_6,                               5)
	TAGKEYS(                        XK_7,                               6)
	TAGKEYS(                        XK_8,                               7)
	TAGKEYS(                        XK_9,                               8)

    /* misc                                                             */
	{ MODKEY,                       XK_n,       togglescratch,          {.v = scratchpadcmd } },
    /*
     * icebucket
     *
     * These shortcuts increase and decrease the number of clients in master side
	 * { MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	 * { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
     *
     * This hotkey is for viewing tags
	 * { MODKEY,                       XK_Tab,    view,           {0} },
     *
     * This hotkey toggles floating mode
	 * { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
     *
     * I have no idea of what these do
     * { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	 * { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	 * { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	 * { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
     *
     * In theory, this command is meant to quit the DWM, but it crashes the environment and doesn't return to TTY
	 * { MODKEY|ShiftMask,             XK_q,      quit,           {0} },
     *
     * These commands are for manipulating the vanitygaps
	 * { MODKEY|Mod4Mask,              XK_u,      incrgaps,       {.i = +1 } },
	 * { MODKEY|Mod4Mask|ShiftMask,    XK_u,      incrgaps,       {.i = -1 } },
	 * { MODKEY|Mod4Mask,              XK_0,      togglegaps,     {0} },
	 * { MODKEY|Mod4Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
     */
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	/* { ClkLtSymbol,          0,              Button1,        setlayout,      {0} }, */
	/* { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} }, */
	/* { ClkWinTitle,          0,              Button2,        zoom,           {0} }, */
	/* { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } }, */
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	/* { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} }, */
	/* { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} }, */
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	/* { ClkTagBar,            MODKEY,         Button1,        tag,            {0} }, */
	/* { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} }, */
};

