#ifndef _DEVTERM_KEYCODE_H_
#define _DEVTERM_KEYCODE_H_

/*
    DevTermKeyCode
  	[for DevTerm]
	Modify : @shikarunochi
	Date   : 2022.02.01-
*/

//https://sdl2referencejp.osdn.jp/SDLKeycodeLookup.html
static uint16_t DevTermKeyCode[] =
{
	0	,	//	0	0	:	
	0	,	//	1	1	:	
	0	,	//	2	2	:	
	0	,	//	3	3	:	
	0	,	//	4	4	:	
	0	,	//	5	5	:	
	0	,	//	6	6	:	
	0	,	//	7	7	:	
	VK_BACK	,	//	8	8	:	BS
	VK_TAB	,	//	9	9	:	TAB
	0	,	//	10	A	:	
	0	,	//	11	B	:	
	0	,	//	12	C	:	
	VK_RETURN	,	//	13	D	:	Enter
	0	,	//	14	E	:	
	0	,	//	15	F	:	
	0	,	//	16	10	:	
	0	,	//	17	11	:	
	0	,	//	18	12	:	
	0	,	//	19	13	:	Pause
	0	,	//	20	14	:	
	0	,	//	21	15	:	
	0	,	//	22	16	:	
	0	,	//	23	17	:	
	0	,	//	24	18	:	
	0	,	//	25	19	:	
	0	,	//	26	1A	:	
	VK_ESCAPE	,	//	27	1B	:	ESC
	0	,	//	28	1C	:	
	0	,	//	29	1D	:	
	0	,	//	30	1E	:	
	0	,	//	31	1F	:	
	VK_SPACE	,	//	32	20	:	SPACE
	0	,	//	33	21	:	
	0	,	//	34	22	:	
	0	,	//	35	23	:	
	0	,	//	36	24	:	
	0	,	//	37	25	:	
	0	,	//	38	26	:	
	VK_OEM_7	,	//	39	27	:	`
	0	,	//	40	28	:	
	0	,	//	41	29	:	
	0	,	//	42	2A	:	
	0	,	//	43	2B	:	
	VK_OEM_COMMA,	//	44	2C	:	,
	VK_OEM_MINUS,	//	45	2D	:	-
	VK_OEM_PERIOD,	//	46	2E	:	.
	VK_OEM_2,	//	47	2F	:	/
	0x30	,	//	48	30	:	0
	0x31	,	//	49	31	:	1
	0x32	,	//	50	32	:	2
	0x33	,	//	51	33	:	3
	0x34	,	//	52	34	:	4
	0x35	,	//	53	35	:	5
	0x36	,	//	54	36	:	6
	0x37	,	//	55	37	:	7
	0x38	,	//	56	38	:	8
	0x39	,	//	57	39	:	9
	0	,	//	58	3A	:	
	VK_OEM_1	,	//	59	3B	:	;
	0	,	//	60	3C	:	
	VK_OEM_PLUS	,	//	61	3D	:	=
	0	,	//	62	3E	:	
	0	,	//	63	3F	:	
	0	,	//	64	40	:	
	0	,	//	65	41	:	
	0	,	//	66	42	:	
	0	,	//	67	43	:	
	0	,	//	68	44	:	
	0	,	//	69	45	:	
	0	,	//	70	46	:	
	0	,	//	71	47	:	
	0	,	//	72	48	:	
	0	,	//	73	49	:	
	0	,	//	74	4A	:	
	0	,	//	75	4B	:	
	0	,	//	76	4C	:	
	0	,	//	77	4D	:	
	0	,	//	78	4E	:	
	0	,	//	79	4F	:	
	0	,	//	80	50	:	
	0	,	//	81	51	:	
	0	,	//	82	52	:	
	0	,	//	83	53	:	
	0	,	//	84	54	:	
	0	,	//	85	55	:	
	0	,	//	86	56	:	
	0	,	//	87	57	:	
	0	,	//	88	58	:	
	0	,	//	89	59	:	
	0	,	//	90	5A	:	
	VK_OEM_4	,	//	91	5B	:	[
	VK_OEM_5	,	//	92	5C	:	
	VK_OEM_6	,	//	93	5D	:	]
	0	,	//	94	5E	:	
	0	,	//	95	5F	:	
	0x60	,	//	96	60	:	
	0x41	,	//	97	61	:	a
	0x42	,	//	98	62	:	b
	0x43	,	//	99	63	:	c
	0x44	,	//	100	64	:	d
	0x45	,	//	101	65	:	e
	0x46	,	//	102	66	:	f
	0x47	,	//	103	67	:	g
	0x48	,	//	104	68	:	h
	0x49	,	//	105	69	:	i
	0x4a	,	//	106	6A	:	j
	0x4b	,	//	107	6B	:	k
	0x4c	,	//	108	6C	:	l
	0x4d	,	//	109	6D	:	m
	0x4e	,	//	110	6E	:	n
	0x4f	,	//	111	6F	:	o
	0x50	,	//	112	70	:	p
	0x51	,	//	113	71	:	q
	0x52	,	//	114	72	:	r
	0x53	,	//	115	73	:	s
	0x54	,	//	116	74	:	t
	0x55	,	//	117	75	:	u
	0x56	,	//	118	76	:	v
	0x57	,	//	119	77	:	w
	0x58	,	//	120	78	:	x
	0x59	,	//	121	79	:	y
	0x5a	,	//	122	7A	:	z
	0	,	//	123	7B	:	
	0	,	//	124	7C	:	
	0	,	//	125	7D	:	
	0	,	//	126	7E	:	
	VK_DELETE	,	//	127	7F	:	DEL
	0	,	//	128	80	:	
	0	,	//	129	81	:	
	0	,	//	130	82	:	
	0	,	//	131	83	:	
	0	,	//	132	84	:	
	0	,	//	133	85	:	
	0	,	//	134	86	:	
	0	,	//	135	87	:	
	0	,	//	136	88	:	
	0	,	//	137	89	:	
	0	,	//	138	8A	:	
	0	,	//	139	8B	:	
	0	,	//	140	8C	:	
	0	,	//	141	8D	:	
	0	,	//	142	8E	:	
	0	,	//	143	8F	:	
	0	,	//	144	90	:	
	0	,	//	145	91	:	
	0	,	//	146	92	:	
	0	,	//	147	93	:	
	0	,	//	148	94	:	
	0	,	//	149	95	:	
	0	,	//	150	96	:	
	0	,	//	151	97	:	
	0	,	//	152	98	:	
	0	,	//	153	99	:	
	0	,	//	154	9A	:	
	0	,	//	155	9B	:	
	0	,	//	156	9C	:	
	0	,	//	157	9D	:	
	0	,	//	158	9E	:	
	0	,	//	159	9F	:	
	0	,	//	160	A0	:	
	0	,	//	161	A1	:	
	0	,	//	162	A2	:	
	0	,	//	163	A3	:	
	0	,	//	164	A4	:	
	0	,	//	165	A5	:	
	0	,	//	166	A6	:	
	0	,	//	167	A7	:	
	0	,	//	168	A8	:	
	0	,	//	169	A9	:	
	0	,	//	170	AA	:	
	0	,	//	171	AB	:	
	0	,	//	172	AC	:	
	0	,	//	173	AD	:	
	0	,	//	174	AE	:	
	0	,	//	175	AF	:	
	0	,	//	176	B0	:	
	0	,	//	177	B1	:	
	0	,	//	178	B2	:	
	0	,	//	179	B3	:	
	0	,	//	180	B4	:	
	0	,	//	181	B5	:	
	0	,	//	182	B6	:	
	0	,	//	183	B7	:	
	0	,	//	184	B8	:	
	0	,	//	185	B9	:	
	0	,	//	186	BA	:	
	0	,	//	187	BB	:	
	0	,	//	188	BC	:	
	0	,	//	189	BD	:	
	0	,	//	190	BE	:	
	0	,	//	191	BF	:	
	0	,	//	192	C0	:	
	0	,	//	193	C1	:	
	0	,	//	194	C2	:	
	0	,	//	195	C3	:	
	0	,	//	196	C4	:	
	0	,	//	197	C5	:	
	0	,	//	198	C6	:	
	0	,	//	199	C7	:	
	0	,	//	200	C8	:	
	0	,	//	201	C9	:	
	0	,	//	202	CA	:	
	0	,	//	203	CB	:	
	0	,	//	204	CC	:	
	0	,	//	205	CD	:	
	0	,	//	206	CE	:	
	0	,	//	207	CF	:	
	0	,	//	208	D0	:	
	0	,	//	209	D1	:	
	0	,	//	210	D2	:	
	0	,	//	211	D3	:	
	0	,	//	212	D4	:	
	0	,	//	213	D5	:	
	0	,	//	214	D6	:	
	0	,	//	215	D7	:	
	0	,	//	216	D8	:	
	0	,	//	217	D9	:	
	0	,	//	218	DA	:	
	0	,	//	219	DB	:	
	0	,	//	220	DC	:	
	0	,	//	221	DD	:	
	0	,	//	222	DE	:	
	0	,	//	223	DF	:	
	0	,	//	224	E0	:	
	0	,	//	225	E1	:	
	0	,	//	226	E2	:	
	0	,	//	227	E3	:	
	0	,	//	228	E4	:	
	0	,	//	229	E5	:	
	0	,	//	230	E6	:	
	0	,	//	231	E7	:	
	0	,	//	232	E8	:	
	0	,	//	233	E9	:	
	0	,	//	234	EA	:	
	0	,	//	235	EB	:	
	0	,	//	236	EC	:	
	0	,	//	237	ED	:	
	0	,	//	238	EE	:	
	0	,	//	239	EF	:	
	0	,	//	240	F0	:	
	0	,	//	241	F1	:	
	0	,	//	242	F2	:	
	0	,	//	243	F3	:	
	0	,	//	244	F4	:	
	0	,	//	245	F5	:	
	0	,	//	246	F6	:	
	0	,	//	247	F7	:	
	0	,	//	248	F8	:	
	0	,	//	249	F9	:	
	0	,	//	250	FA	:	
	0	,	//	251	FB	:	
	0	,	//	252	FC	:	
	0	,	//	253	FD	:	
	0	,	//	254	FE	:	
	0		//	255	FF	:	
};

static unsigned int DevTermFuncKeyCode[][2] ={
	
	{SDLK_CAPSLOCK,VK_CAPITAL},
	{SDLK_F1,VK_F1},
	{SDLK_F2,VK_F2},
	{SDLK_F3,VK_F3},
	{SDLK_F4,VK_F4},
	{SDLK_F5,VK_F5},
	{SDLK_F6,VK_F6},
	{SDLK_F7,VK_F7},
	{SDLK_F8,VK_F8},
	{SDLK_F9,VK_F9},
	{SDLK_F10,VK_F10},
	{SDLK_F11,VK_F11},
	{SDLK_F12,VK_F12},
	{SDLK_PRINTSCREEN,VK_PRINT},
	{SDLK_SCROLLLOCK,0},
	{SDLK_PAUSE,0},
	{SDLK_INSERT,VK_INSERT},
	{SDLK_HOME,VK_HOME},
	{SDLK_PAGEUP,VK_PRIOR},
	{SDLK_END,VK_END},
	{SDLK_PAGEDOWN,VK_NEXT},
	{SDLK_RIGHT,VK_RIGHT},
	{SDLK_LEFT,VK_LEFT},
	{SDLK_DOWN,VK_DOWN},
	{SDLK_UP,VK_UP},
	{SDLK_NUMLOCKCLEAR,0},
	{SDLK_KP_DIVIDE,0},
	{SDLK_KP_MULTIPLY,0},
	{SDLK_KP_MINUS,0},
	{SDLK_KP_PLUS,0},
	{SDLK_KP_ENTER,0},
	{SDLK_KP_1,0},
	{SDLK_KP_2,0},
	{SDLK_KP_3,0},
	{SDLK_KP_4,0},
	{SDLK_KP_5,0},
	{SDLK_KP_6,0},
	{SDLK_KP_7,0},
	{SDLK_KP_8,0},
	{SDLK_KP_9,0},
	{SDLK_KP_0,0},
	{SDLK_KP_PERIOD,0},
	{SDLK_APPLICATION,0},
	{SDLK_POWER,0},
	{SDLK_KP_EQUALS,0},
	{SDLK_F13,0},
	{SDLK_F14,0},
	{SDLK_F15,0},
	{SDLK_F16,0},
	{SDLK_F17,0},
	{SDLK_F18,0},
	{SDLK_F19,0},
	{SDLK_F20,0},
	{SDLK_F21,0},
	{SDLK_F22,0},
	{SDLK_F23,0},
	{SDLK_F24,0},
	{SDLK_EXECUTE,0},
	{SDLK_HELP,0},
	{SDLK_MENU,0},
	{SDLK_SELECT,0},
	{SDLK_STOP,0},
	{SDLK_AGAIN,0},
	{SDLK_UNDO,0},
	{SDLK_CUT,0},
	{SDLK_COPY,0},
	{SDLK_PASTE,0},
	{SDLK_FIND,0},
	{SDLK_MUTE,0},
	{SDLK_VOLUMEUP,0},
	{SDLK_VOLUMEDOWN,0},
	{SDLK_KP_COMMA,0},
	{SDLK_KP_EQUALSAS400,0},
	{SDLK_ALTERASE,0},
	{SDLK_SYSREQ,0},
	{SDLK_CANCEL,0},
	{SDLK_CLEAR,0},
	{SDLK_PRIOR,0},
	{SDLK_RETURN2,0},
	{SDLK_SEPARATOR,0},
	{SDLK_OUT,0},
	{SDLK_OPER,0},
	{SDLK_CLEARAGAIN,0},
	{SDLK_CRSEL,0},
	{SDLK_EXSEL,0},
	{SDLK_KP_00,0},
	{SDLK_KP_000,0},
	{SDLK_THOUSANDSSEPARATOR,0},
	{SDLK_DECIMALSEPARATOR,0},
	{SDLK_CURRENCYUNIT,0},
	{SDLK_CURRENCYSUBUNIT,0},
	{SDLK_KP_LEFTPAREN,0},
	{SDLK_KP_RIGHTPAREN,0},
	{SDLK_KP_LEFTBRACE,0},
	{SDLK_KP_RIGHTBRACE,0},
	{SDLK_KP_TAB,0},
	{SDLK_KP_BACKSPACE,0},
	{SDLK_KP_A,0},
	{SDLK_KP_B,0},
	{SDLK_KP_C,0},
	{SDLK_KP_D,0},
	{SDLK_KP_E,0},
	{SDLK_KP_F,0},
	{SDLK_KP_XOR,0},
	{SDLK_KP_POWER,0},
	{SDLK_KP_PERCENT,0},
	{SDLK_KP_LESS,0},
	{SDLK_KP_GREATER,0},
	{SDLK_KP_AMPERSAND,0},
	{SDLK_KP_DBLAMPERSAND,0},
	{SDLK_KP_VERTICALBAR,0},
	{SDLK_KP_DBLVERTICALBAR,0},
	{SDLK_KP_COLON,0},
	{SDLK_KP_HASH,0},
	{SDLK_KP_SPACE,0},
	{SDLK_KP_AT,0},
	{SDLK_KP_EXCLAM,0},
	{SDLK_KP_MEMSTORE,0},
	{SDLK_KP_MEMRECALL,0},
	{SDLK_KP_MEMCLEAR,0},
	{SDLK_KP_MEMADD,0},
	{SDLK_KP_MEMSUBTRACT,0},
	{SDLK_KP_MEMMULTIPLY,0},
	{SDLK_KP_MEMDIVIDE,0},
	{SDLK_KP_PLUSMINUS,0},
	{SDLK_KP_CLEAR,0},
	{SDLK_KP_CLEARENTRY,0},
	{SDLK_KP_BINARY,0},
	{SDLK_KP_OCTAL,0},
	{SDLK_KP_DECIMAL,0},
	{SDLK_KP_HEXADECIMAL,0},
	{SDLK_LCTRL,VK_LCONTROL},
	{SDLK_LSHIFT,VK_LSHIFT},
	{SDLK_LALT,VK_LMENU},
	{SDLK_LGUI,0},
	{SDLK_RCTRL,VK_RCONTROL},
	{SDLK_RSHIFT,VK_RSHIFT},
	{SDLK_RALT,VK_RMENU},
	{SDLK_RGUI,VK_APPS},
	{SDLK_MODE,0},
	{SDLK_AUDIONEXT,0},
	{SDLK_AUDIOPREV,0},
	{SDLK_AUDIOSTOP,0},
	{SDLK_AUDIOPLAY,0},
	{SDLK_AUDIOMUTE,0},
	{SDLK_MEDIASELECT,0},
	{SDLK_WWW,0},
	{SDLK_MAIL,0},
	{SDLK_CALCULATOR,0},
	{SDLK_COMPUTER,0},
	{SDLK_AC_SEARCH,0},
	{SDLK_AC_HOME,0},
	{SDLK_AC_BACK,0},
	{SDLK_AC_FORWARD,0},
	{SDLK_AC_STOP,0},
	{SDLK_AC_REFRESH,0},
	{SDLK_AC_BOOKMARKS,0},
	{SDLK_BRIGHTNESSDOWN,0},
	{SDLK_BRIGHTNESSUP,0},
	{SDLK_DISPLAYSWITCH,0},
	{SDLK_KBDILLUMTOGGLE,0},
	{SDLK_KBDILLUMDOWN,0},
	{SDLK_KBDILLUMUP,0},
	{SDLK_EJECT,0},
	{SDLK_SLEEP,0},
	{0,0}
};
#endif