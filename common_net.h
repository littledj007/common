// ver 1.0
// modified: 
//   2014-12-17,littledj: 去除非common内容
//   2015-05-23,littledj: 修改判断越界条件

#pragma once
//#include <windows.h>
#include "common.h"
#pragma warning(disable:4200)

#define MAX_PACKET_LEN 2000

/********************************************************************
* DNS
********************************************************************/
#define DNS_QTYPE_A         0x0001
#define DNS_QTYPE_SOA       0x0006
#define DNS_QTYPE_SRV       0x0021

#define DNS_FLAG_IS_RESPONSE        0x8000
#define DNS_FLAG_OPCODE_MASK        0x7800
#define DNS_FLAG_IS_AUTHORITY       0x0400
#define DNS_FLAG_IS_TRUNCATED       0x0200
#define DNS_FLAG_IS_RECURSION_D     0x0100
#define DNS_FLAG_IS_RECURSION_A     0x0080
#define DNS_FLAG_RESERVED           0x0040
#define DNS_FLAG_IS_ANSWER_AUTH     0x0020
#define DNS_FLAG_IS_NONAUTH_ACCEPT  0x0010
#define DNS_FLAG_REPLYCODE_MASK     0x000f

#pragma pack(push,1)

typedef struct _dns_head
{
    unsigned short trans_id;
    unsigned short flags;
    unsigned short questions;   // question 数量
    unsigned short answer;      // answer 数量
    unsigned short authority;   // authority 数量
    unsigned short additional;  // additional 数量
    unsigned char data[0];
}DNS_HEAD, *PDNS_HEAD;

typedef struct _dns_answer_head
{
    unsigned short name;
    unsigned short type;
    unsigned short _class;
    unsigned long ttl;
    unsigned short datalen;
}DNS_ANSWER_HEAD;

typedef struct _dns_answer_A
{
    unsigned long addr[0];
}DNS_ANSWER_A;

typedef struct _dns_answer_SRV
{
    unsigned short priority;
    unsigned short weight;
    unsigned short port;
    unsigned char target[0];
}DNS_ANSWER_SRV;

typedef struct _iphdr //定义IP首部 
{
    unsigned char h_lenver; //4位首部长度+4位IP版本号 
    unsigned char tos; //8位服务类型TOS 
    unsigned short total_len; //16位总长度（字节） 
    unsigned short ident; //16位标识 
    unsigned short frag_and_flags; //3位标志位 
    unsigned char ttl; //8位生存时间 TTL 
    unsigned char proto; //8位协议 (TCP, UDP 或其他) 
    unsigned short checksum; //16位IP首部校验和 
    unsigned int sourceIP; //32位源IP地址 
    unsigned int destIP; //32位目的IP地址 
}IP_HEADER;

typedef struct psd_hdr //定义TCP伪首部 
{
    unsigned long saddr; //源地址 
    unsigned long daddr; //目的地址 
    char mbz;
    char ptcl; //协议类型 
    unsigned short tcpl; //TCP长度 
}PSD_HEADER;

typedef struct _tcphdr //定义TCP首部 
{
    unsigned short th_sport; //16位源端口 
    unsigned short th_dport; //16位目的端口 
    unsigned int th_seq; //32位序列号 
    unsigned int th_ack; //32位确认号 
    unsigned char th_lenres; //4位首部长度/6位保留字 
    unsigned char th_flag; //6位标志位 
    unsigned short th_win; //16位窗口大小 
    unsigned short th_sum; //16位校验和 
    unsigned short th_urp; //16位紧急数据偏移量 
}TCP_HEADER;

#pragma pack(pop)

unsigned short PKT_CheckSum(unsigned short *buf, int bufSize)
{
    unsigned long cksum = 0;
    while (bufSize > 1)
    {
        cksum += *buf++;
        bufSize -= sizeof(unsigned short);
    }
    if (bufSize)
    {
        cksum += *(unsigned char*)buf;
    }
    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >> 16);
    return (unsigned short)(~cksum);
}

/********************************************************************
* [函数名]: PKT_AddByte
* [描述]: 向网络缓冲区添加指定数量的字节
* [输入]:
*   buf: 网络缓冲区指针
*   bufSize: 缓冲区大小
*   pos: 添加位置
*   data: 待添加的字节
*   count: 字节数量
* [输出]:
*   pos: 添加数据后的尾部位置
* [返回值]: int
*   1：成功
*   0: 未添加任何数据
*   -1：失败
* [修改记录]:
*   2014-12-17,littledj: create
********************************************************************/
int static PKT_AddByte(char* buf, int bufSize, int &pos, unsigned char data, int count = 1)
{
    if (buf == NULL || pos + count >= bufSize)
    {
        return -1;
    }

    memset(buf + pos, data, count);
    pos += count;
    return count;
}

int static PKT_SetWORD(char* buf, int bufSize, int pos, unsigned short data)
{
    int data_size = sizeof(short);
    if (buf == NULL || pos + data_size >= bufSize)
    {
        return -1;
    }

    unsigned short data_net = gcommon::g_htons(data);
    memcpy(buf + pos, (char*)&data_net, data_size);
    return data_size;
}

/********************************************************************
* [函数名]: PKT_AddWORD
* [描述]: 向网络缓冲区添加WORD数据
* [输入]:
*   buf: 网络缓冲区指针
*   bufSize: 缓冲区大小
*   pos: 添加位置
*   data: 待添加的数据
* [输出]:
*   pos: 添加数据后的尾部位置
* [返回值]: int
*   >0：成功,添加的字节数
*   0: 未添加任何数据
*   -1：失败
* [修改记录]:
*   2014-10-21,littledj: create
********************************************************************/
int static PKT_AddWORD(char* buf, int bufSize, int &pos, unsigned short data)
{
    int data_size = sizeof(short);
    PKT_SetWORD(buf, bufSize, pos, data);
    pos += data_size;
    return data_size;
}

/********************************************************************
* [函数名]: PKT_AddDWORD
* [描述]: 向网络缓冲区添加DWORD数据
* [输入]:
*   buf: 网络缓冲区指针
*   bufSize: 缓冲区大小
*   pos: 添加位置
*   data: 待添加的数据
* [输出]:
*   pos: 添加数据后的尾部位置
* [返回值]: int
*   >0：成功,添加的字节数
*   0: 未添加任何数据
*   -1：失败
* [修改记录]:
*   2014-10-21,littledj: create
********************************************************************/
int static PKT_AddDWORD(char* buf, int bufSize, int &pos, unsigned long data)
{
    int data_size = sizeof(long);
    if (buf == NULL || pos + data_size >= bufSize)
    {
        return -1;
    }

    unsigned long data_net = gcommon::g_htonl(data);
    memcpy(buf + pos, (char*)&data_net, data_size);
    pos += data_size;
    return data_size;
}

/********************************************************************
* [函数名]: PKT_AddBytes
* [描述]: 向网络缓冲区添加字符串
* [输入]:
*   buf: 网络缓冲区指针
*   bufSize: 缓冲区大小
*   pos: 添加位置
*   data: 待添加的字符串
*   data_size: 字符串长度
* [输出]:
*   pos: 添加数据后的尾部位置
* [返回值]: int
*   >0：成功,添加的字节数
*   0: 未添加任何数据
*   -1：失败
* [修改记录]:
*   2014-12-17,littledj: create
********************************************************************/
int static PKT_AddBytes(char* buf, int bufSize, int &pos, char* data, int data_size)
{
    if (buf == NULL || pos + data_size >= bufSize || data == NULL)
    {
        return -1;
    }

    memcpy(buf + pos, data, data_size);
    pos += data_size;
    return data_size;
}

/********************************************************************
* [函数名]: PKT_AddNop
* [描述]: 向网络缓冲区添加‘\0’填充
* [输入]:
*   buf: 网络缓冲区指针
*   bufSize: 缓冲区大小
*   pos: 添加位置
*   count: 添加‘\0’的数量
* [输出]:
*   pos: 添加数据后的尾部位置
* [返回值]: int
*   1：成功,添加的字节数
*   0: 未添加任何数据
*   -1：失败
* [修改记录]:
*   2014-12-17,littledj: create
********************************************************************/
int static PKT_AddNop(char* buf, int bufSize, int &pos, int count = 1)
{
    if (buf == NULL || pos + count >= bufSize)
    {
        return -1;
    }

    memset(buf + pos, 0, count);
    pos += count;
    return count;
}