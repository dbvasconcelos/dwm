/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx       = 4;  /* border pixel of windows */
static const unsigned int snap           = 32; /* snap pixel */
static unsigned int gappih               = 10; /* horiz inner gap between windows */
static unsigned int gappiv               = 10; /* vert inner gap between windows */
static unsigned int gappoh               = 10; /* horiz outer gap between windows and screen edge */
static unsigned int gappov               = 10; /* vert outer gap between windows and screen edge */
static int smartgaps                     = 0;  /* 1 means no outer gap when there is only one window */
static const int showbar                 = 1;  /* 0 means no bar */
static const int topbar                  = 1;  /* 0 means bottom bar */
static const int showsystray             = 1;   /* 0 means no systray */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const char *fonts[]               = { "monospace:size=10:style=bold", "Noto Color Emoji:size=10" };
static char normbgcolor[]                = "#222222";
static char normbordercolor[]            = "#444444";
static char normfgcolor[]                = "#bbbbbb";
static char selfgcolor[]                 = "#eeeeee";
static char selbordercolor[]             = "#005577";
static char selbgcolor[]                 = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

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
};

/* floating */
static const int swallowfloating    = 0;   /* 1 means swallow floating windows by default */
static int floatposgrid_x           = 5;   /* float grid columns */
static int floatposgrid_y           = 5;   /* float grid rows */

/* scratchpads */
typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "terminal", "-g", "120x34", NULL };
const char *spcmd2[] = {"st", "-n", "filemanager", "-g", "144x41", "-e", "lf", NULL };
const char *spcmd3[] = {"st", "-n", "calculator", "-g", "30x20", "-e", "bc", "-l", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spranger",    spcmd2},
	{"keepassxc",   spcmd3},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const Rule rules[] = {
	/* xprop(1):
	 *  WM_CLASS(STRING) = instance, class
	 *  WM_NAME(STRING) = title
	 *  WM_WINDOW_ROLE(STRING) = role
	 */
	/* class     role     instance       title           tags mask  iscentered  isfloating  isterminal  noswallow  floatpos  monitor */
	{ "Gimp",    NULL,    NULL,          NULL,           0,         0,          1,          0,           0,        NULL,     -1 },
	{ NULL,      NULL,    "terminal",    NULL,           SPTAG(0),  1,          1,          1,           0,        NULL,     -1 },
	{ NULL,      NULL,    "filemanager", NULL,           SPTAG(1),  1,          1,          1,           0,        NULL,     -1 },
	{ NULL,      NULL,    "calculator",  NULL,           SPTAG(2),  1,          1,          1,           0,        NULL,     -1 },
};

/* layout(s) */
static float mfact           = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster           = 1;    /* number of clients in master area */
static int resizehints       = 0;    /* 1 means respect size hints in tiled resizals */
static const int decorhints  = 1;    /* 1 means respect decoration hints */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "|||",      col },
	{ "[D]",      deck },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
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
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \
	{ MOD, XK_q,     ACTION##stack, {.i = 0 } }, \
	{ MOD, XK_a,     ACTION##stack, {.i = 1 } }, \
	{ MOD, XK_z,     ACTION##stack, {.i = 2 } }, \
	{ MOD, XK_x,     ACTION##stack, {.i = -1 } },

static Key keys[] = {
	{0},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask        button          function        argument */
	{ ClkLtSymbol,          0,                Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,                Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,                Button2,        zoom,           {0} },
	{ ClkStatusText,        0,                Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,                Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,                Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkStatusText,        ShiftMask,        Button1,        sigdwmblocks,   {.i = 6} },
	{ ClkClientWin,         MODKEY,           Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,           Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,           Button3,        resizemouse,    {0} },
	{ ClkClientWin,         MODKEY|ShiftMask, Button1,        dragmfact,      {0} },
	{ ClkClientWin,         MODKEY|ShiftMask, Button3,        dragcfact,      {0} },
	{ ClkTagBar,            0,                Button1,        view,           {0} },
	{ ClkTagBar,            0,                Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,           Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,           Button3,        toggletag,      {0} },
};

/* signal definitions */
/* signum must be greater than 0 */
/* trigger signals using `xsetroot -name "fsignal:<signame> [<type> <value>]"` */
static Signal signals[] = {
	/* signum           function */
	{ "defaultgaps",    defaultgaps },
	{ "focusmon",       focusmon },
	{ "focusstack",     focusstack },
	{ "incnmaster",     incnmaster },
	{ "incrgaps",       incrgaps },
	{ "killclient",     killclient },
	{ "pushstack",      pushstack },
	{ "quit",           quit },
	{ "setcfact",       setcfact },
	{ "setgaps",        setgapsex },
	{ "setlayout",      setlayout },
	{ "setlayoutex",    setlayoutex },
	{ "setmfact",       setmfact },
	{ "shiftview",      shiftviewclients },
	{ "tag",            tag },
	{ "tagall",         tagall },
	{ "tagex",          tagex },
	{ "tagmon",         tagmon },
	{ "togglebar",      togglebar },
	{ "togglefloating", togglefloating },
	{ "togglefullscr",  togglefullscr },
	{ "togglegaps",     togglegaps },
	{ "togglescratch",  togglescratch },
	{ "togglesticky",   togglesticky },
	{ "toggletag",      tag },
	{ "toggletagex",    toggletagex },
	{ "toggleview",     view },
	{ "toggleviewex",   toggleviewex },
	{ "view",           view },
	{ "viewall",        viewall },
	{ "viewex",         viewex },
	{ "zoom",           zoom },
};
