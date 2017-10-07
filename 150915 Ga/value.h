#pragma once

const DWORD STATE_STAND		=	0x00000001;
const DWORD STATE_MOVE		=	0x00000002;
const DWORD STATE_DASH		=	0x00000004;
const DWORD	STATE_DEAD		=	0x00100000;

const DWORD STATE_ATTACK1	=	0x00000010;
const DWORD STATE_ATTACK2	=	0x00000020;
const DWORD STATE_ATTACK3	=	0x00000040;
const DWORD STATE_ATTACK4	=	0x00000080;

const DWORD STATE_STRIKE1	=	0x00000100;
const DWORD STATE_STRIKE2	=	0x00000200;
const DWORD STATE_STRIKE3	=	0x00000400;
const DWORD STATE_STRIKE4	=	0x00000800;
const DWORD STATE_STRIKE5	=	0x00001000;

const DWORD STATE_ATTACK	=	0x00010000;
const DWORD STATE_CAST		=	0x00020000;
const DWORD STATE_SKILL		=	0x00040000;

const DWORD KEY_UP		=	0x00000001;
const DWORD KEY_DOWN	=	0x00000002;
const DWORD KEY_LEFT	=	0x00000004;
const DWORD KEY_RIGHT	=	0x00000008;
const DWORD KEY_SPACE	=	0x00000010;
const DWORD KEY_RETURN	=	0x00000020;
const DWORD KEY_LBUTTON	=	0x00000040;
const DWORD KEY_A		=	0x00000080;
const DWORD KEY_S		=	0x00000100;
const DWORD KEY_D		=	0x00000200;
const DWORD KEY_ESC		=	0x00000400;