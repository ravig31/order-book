# Trading Order Types Overview

## Orders Requiring Both Price & Quantity
1. **Limit Orders**
  - Set exact price & quantity
  - Won't execute beyond specified price
  - Example: Buy 100 shares at max $50

2. **Fill or Kill (FOK)**
  - Set exact price & quantity
  - Must fill completely or cancels
  - No partial fills allowed

3. **Fill and Kill (FAK)**
  - Set exact price & quantity
  - Accepts partial fills
  - Cancels unfilled portion

## Orders Requiring Only Quantity
1. **Market Orders**
  - Specify only quantity
  - Executes at current market price
  - Highest execution priority

2. **Good Till Cancelled (GTC)**
  - Remains active until manually cancelled
  - Can apply to market or limit orders
  - Some brokers auto-cancel after 30-90 days


# Todo

[] Implement removing good for day orders 
[] Unit tests
[] Load testing
