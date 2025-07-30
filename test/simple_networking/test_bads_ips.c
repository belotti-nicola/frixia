#include "../../src/utils/networking/network.h"
#include <stdio.h>

int main()
{
    if ( is_valid_ipv4("256.1.1.1") == true )
    {
        printf("error test_bads_ips: line %d\n",__LINE__);
        return 1;
    }

    if ( is_valid_ipv4("192.168.1") == true )
    {
        printf("error test_bads_ips: line %d\n",__LINE__);
        return 1;
    }

    if ( is_valid_ipv4("192.168.1.1.1") == true )
    {
        printf("error test_bads_ips: line %d\n",__LINE__);
        return 1;
    }

    if ( is_valid_ipv4("192.168.-1.1") == true )
    {
        printf("error test_bads_ips: line %d\n",__LINE__);
        return 1;
    }

    if ( is_valid_ipv4("192.168.01.1") == true )
    {
        printf("error test_bads_ips: line %d\n",__LINE__);
        return 1;
    }

    if ( is_valid_ipv4("192.168.1.a") == true ) 
    {
        printf("error test_bads_ips: line %d\n",__LINE__);
        return 1;
    }

    if ( is_valid_ipv4("192.168..1") == true ) 
    {
        printf("error test_bads_ips: line %d\n",__LINE__);
        return 1;
    }

    if ( is_valid_ipv4(" 192.168.1.1") == true )
    {
        printf("error test_bads_ips: line %d\n",__LINE__);
        return 1;
    }

    if ( is_valid_ipv4("192.168.1.1 ") == true )
    {
        printf("error test_bads_ips: line %d\n",__LINE__);
        return 1;
    }

    if ( is_valid_ipv4("hello.world.test.com") == true )
    {
        printf("error test_bads_ips: line %d\n",__LINE__);
        return 1;
    }

    if ( is_valid_ipv4("108.274.2.41") == true )
    {
        printf("error test_goods_ips: line%d\n",__LINE__);
        return 1;
    }

    if ( is_valid_ipv4("75.1.723.41") == true )
    {
        printf("error test_goods_ips: line%d\n",__LINE__);
        return 1;
    }

    if ( is_valid_ipv4("75.1.2..41") == true )
    {
        printf("error test_goods_ips: line%d\n",__LINE__);
        return 1;
    }

    if ( is_valid_ipv4("36.1.2.257") == true )
    {
        printf("error test_goods_ips: line%d\n",__LINE__);
        return 1;
    }

    return 0;
}