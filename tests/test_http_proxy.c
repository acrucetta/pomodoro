#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unity.h>
#include "http_proxy.h"

// Mock ClientHello data with known SNI
unsigned char client_hello_with_sni[] = {
    // ... mock data representing a TLS ClientHello message with a specific SNI ...
};

// Test function for extract_sni
void test_extractSNI_ShouldExtractCorrectSNI(void) {
    char* extracted_sni = extract_sni(client_hello_with_sni, sizeof(client_hello_with_sni));
    TEST_ASSERT_NOT_NULL(extracted_sni);
    TEST_ASSERT_EQUAL_STRING("expected_sni.com", extracted_sni);
    free(extracted_sni); // Assuming extract_sni dynamically allocates the returned string
}

// More test functions can be added here for different scenarios

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_extractSNI_ShouldExtractCorrectSNI);
    // ... Run other test functions ...
    return UNITY_END();
}
