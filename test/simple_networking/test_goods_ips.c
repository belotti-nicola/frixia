#include "../../src/utils/networking/network.h"
#include <stdio.h>

int main()
{
    if ( is_valid_ipv4("192.168.100.200") == false )
    {
        printf("error test_goods_ips: line%d\n",__LINE__);
        return 1;
    }

    if ( is_valid_ipv4("1.1.1.1") == false )
    {
        printf("error test_goods_ips: line%d\n",__LINE__);
        return 1;
    }

    if ( is_valid_ipv4("0.0.0.0") == false )
    {
        printf("error test_goods_ips: line%d\n",__LINE__);
        return 1;
    }

    if ( is_valid_ipv4("10.10.10.10") == false )
    {
        printf("error test_goods_ips: line%d\n",__LINE__);
        return 1;
    }

    if ( is_valid_ipv4("192.168.1.1") == false )
    {
        printf("error test_goods_ips: line%d\n",__LINE__);
        return 1;
    }
    
    if ( is_valid_ipv4("10.0.0.1") == false )
    {
        printf("error test_goods_ips: line%d\n",__LINE__);
        return 1;
    }

    if ( is_valid_ipv4("172.16.254.1") == false )
    {
        printf("error test_goods_ips: line%d\n",__LINE__);
        return 1;
    }

    if ( is_valid_ipv4("8.8.8.8") == false )
    {
        printf("error test_goods_ips: line%d\n",__LINE__);
        return 1;
    }

    if ( is_valid_ipv4("255.255.255.255") == false )
    {
        printf("error test_goods_ips: line%d\n",__LINE__);
        return 1;
    }

    if ( is_valid_ipv4("203.0.113.1") == false )
    {
        printf("error test_goods_ips: line%d\n",__LINE__);
        return 1;
    }

    if ( is_valid_ipv4("198.51.100.42") == false )
    {
        printf("error test_goods_ips: line%d\n",__LINE__);
        return 1;
    }

    return 0;
}