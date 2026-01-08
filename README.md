Markdown# Digital Logic Truth Table Solver

A simple, colorful console-based C++ program that generates complete **truth tables** for any logical expression with up to **6 variables**. It evaluates the expression, displays the truth table, and classifies the expression as **Tautology**, **Contradiction**, or **Contingent**.

Perfect for students learning Discrete Mathematics, Digital Logic Design, or Boolean Algebra.

## Features

- Supports the following operators:
  - `&` → AND
  - `|` → OR
  - `!` → NOT
  - `>` → IMPLIES (→)
- Handles parentheses for grouping: `(p & q) | (!p & r)`
- Automatically detects and sorts variables (a–z)
- Supports up to **6 variables** (64 rows maximum)
- Colorful output using ANSI escape codes (works on most terminals)
- Classifies the expression:
  - **Tautology** (always true)
  - **Contradiction** (always false)
  - **Contingent** (depends on inputs)
- Shows percentage of true/false outcomes
- Includes a real-world example: **Fraud Detection Rule**

## How to Use

1. **Compile the program**
   ```bash
   g++ dm_pro.cpp -o logic_solver

Run the executableBash./logic_solver
Enter a logical expression when prompted. Examples:textp & q
p | !q
(p > q) & (q > p)
(a & b) | (!a & c)
(p & q) > r
The program will display:
All variable combinations
Truth table with colored T/F
Analysis (Tautology / Contradiction / Contingent)
True/False statistics

Type quit to exit.

Supported Syntax Rules

Use lowercase letters a to z as variables
Operators: & (AND),  (OR), ! (NOT), > (IMPLIES)
Use parentheses () for grouping
No spaces needed (but allowed in some cases)
NOT has highest precedence, then AND, then OR, then IMPLIES

ExpressionMeaningp & qp AND q`pq`!pNOT pp > qp IMPLIES q`(p & q)(!p & r)`(a > b) & (b > c)Transitivity check
Real-World Example (Included in Program)
The program demonstrates a simple fraud detection rule:
text(l & n) | (c & r)
Where:

l = large transaction
n = new customer
c = cross-border
r = high-risk country

If either condition is true → Fraud Alert
Limitations

Maximum 6 variables (due to 2⁶ = 64 rows)
Does not support XOR (^) or EQUIVALENCE (=) directly
Parentheses must be properly balanced

Compilation Notes

Uses ANSI escape codes for colors – works best on Linux/macOS terminals and modern Windows Terminal / Windows 10+
On older Windows CMD, colors may not appear (but program still works)

Contributing
Feel free to fork and improve! Possible enhancements:

Add XOR and EQUIVALENCE operators
Export truth table to CSV
GUI version
Support for more than 6 variables (with pagination)

License
This project is open-source and free to use for educational purposes.

Made by Aryan Ahmad (aryanahmadfiaz@gmail.com)
