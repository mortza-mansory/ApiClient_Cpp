#ifndef PRODUCT_SERVICE_H
#define PRODUCT_SERVICE_H

#include <string>

class ProductService {
public:
    static std::string GetProductInfo(int productId);
};
#endif // PRODUCT_SERVICE_H