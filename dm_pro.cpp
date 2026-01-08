#include <iostream>
#include <string>
using namespace std;

// ANSI color codes
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define RESET "\033[0m"

// Global arrays
bool table[64][6]; // Truth table values
bool results[64];  // Results
char vars[6];      // Variable names
int varCount;      // Number of variables

// Helper function to get variable index
int getVarIndex(char var)
{
    for (int i = 0; i < varCount; i++)
    {
        if (vars[i] == var)
            return i;
    }
    return -1;
}

// Function to extract variables from expression
void getVariables(string expr)
{
    bool found[26] = {false};
    varCount = 0;

    for (int i = 0; i < expr.length(); i++)
    {
        char c = expr[i];
        if (c >= 'a' && c <= 'z' && !found[c - 'a'])
        {
            vars[varCount] = c;
            found[c - 'a'] = true;
            varCount++;
        }
    }

    // Sort alphabetically
    for (int i = 0; i < varCount - 1; i++)
    {
        for (int j = i + 1; j < varCount; j++)
        {
            if (vars[i] > vars[j])
            {
                char temp = vars[i];
                vars[i] = vars[j];
                vars[j] = temp;
            }
        }
    }
}

// Function to get value of a character (T, F, or variable)
bool getValue(char c, bool values[])
{
    if (c == 'T')
        return true;
    if (c == 'F')
        return false;
    int idx = getVarIndex(c);
    if (idx >= 0)
        return values[idx];
    return false; // Should never reach here for valid expressions
}

// Function to evaluate a simple expression (no parentheses)
bool evalSimple(string expr, bool values[])
{
    // Create a working copy
    string work = expr;

    // First pass: Handle NOT operations
    while (true)
    {
        bool changed = false;
        for (int i = 0; i < work.length(); i++)
        {
            if (work[i] == '!' && i + 1 < work.length())
            {
                char next = work[i + 1];
                if (next == 'T' || next == 'F' || (next >= 'a' && next <= 'z'))
                {
                    bool val = getValue(next, values);
                    bool result = !val;
                    string replaceWith = result ? "T" : "F";
                    work.replace(i, 2, replaceWith);
                    changed = true;
                    break;
                }
            }
        }
        if (!changed)
            break;
    }

    // Second pass: Handle AND operations
    while (true)
    {
        bool changed = false;
        for (int i = 0; i < work.length(); i++)
        {
            if (work[i] == '&' && i > 0 && i + 1 < work.length())
            {
                char left = work[i - 1];
                char right = work[i + 1];
                if ((left == 'T' || left == 'F' || (left >= 'a' && left <= 'z')) &&
                    (right == 'T' || right == 'F' || (right >= 'a' && right <= 'z')))
                {

                    bool leftVal = getValue(left, values);
                    bool rightVal = getValue(right, values);
                    bool result = leftVal && rightVal;
                    string replaceWith = result ? "T" : "F";
                    work.replace(i - 1, 3, replaceWith);
                    changed = true;
                    break;
                }
            }
        }
        if (!changed)
            break;
    }

    // Third pass: Handle OR operations
    while (true)
    {
        bool changed = false;
        for (int i = 0; i < work.length(); i++)
        {
            if (work[i] == '|' && i > 0 && i + 1 < work.length())
            {
                char left = work[i - 1];
                char right = work[i + 1];
                if ((left == 'T' || left == 'F' || (left >= 'a' && left <= 'z')) &&
                    (right == 'T' || right == 'F' || (right >= 'a' && right <= 'z')))
                {

                    bool leftVal = getValue(left, values);
                    bool rightVal = getValue(right, values);
                    bool result = leftVal || rightVal;
                    string replaceWith = result ? "T" : "F";
                    work.replace(i - 1, 3, replaceWith);
                    changed = true;
                    break;
                }
            }
        }
        if (!changed)
            break;
    }

    // Fourth pass: Handle IMPLIES operations
    while (true)
    {
        bool changed = false;
        for (int i = 0; i < work.length(); i++)
        {
            if (work[i] == '>' && i > 0 && i + 1 < work.length())
            {
                char left = work[i - 1];
                char right = work[i + 1];
                if ((left == 'T' || left == 'F' || (left >= 'a' && left <= 'z')) &&
                    (right == 'T' || right == 'F' || (right >= 'a' && right <= 'z')))
                {

                    bool leftVal = getValue(left, values);
                    bool rightVal = getValue(right, values);
                    bool result = (!leftVal) || rightVal;
                    string replaceWith = result ? "T" : "F";
                    work.replace(i - 1, 3, replaceWith);
                    changed = true;
                    break;
                }
            }
        }
        if (!changed)
            break;
    }

    // Final result
    if (work.length() == 1)
    {
        return getValue(work[0], values);
    }

    // If we still have something complex, try to evaluate it
    // This handles cases where we might have a single variable left
    bool finalResult = false;
    for (int i = 0; i < work.length(); i++)
    {
        if (work[i] >= 'a' && work[i] <= 'z')
        {
            int idx = getVarIndex(work[i]);
            if (idx >= 0)
                finalResult = values[idx];
            break;
        }
    }

    return finalResult;
}

// Function to handle parentheses
string evalParentheses(string expr, bool values[])
{
    string work = expr;

    while (true)
    {
        int start = -1, end = -1;
        for (int i = 0; i < work.length(); i++)
        {
            if (work[i] == '(')
                start = i;
            if (work[i] == ')' && start != -1)
            {
                end = i;
                break;
            }
        }
        if (start == -1 || end == -1)
            break;

        string inside = work.substr(start + 1, end - start - 1);
        bool result = evalSimple(inside, values);
        string replaceWith = result ? "T" : "F";
        work.replace(start, end - start + 1, replaceWith);
    }

    return work;
}

// Main evaluation
bool evaluate(string expr, bool values[])
{
    string simplified = evalParentheses(expr, values);
    return evalSimple(simplified, values);
}

// Generate truth table
void makeTruthTable(string expr)
{
    int totalRows = 1;
    for (int i = 0; i < varCount; i++)
        totalRows *= 2;

    for (int row = 0; row < totalRows; row++)
    {
        for (int v = 0; v < varCount; v++)
        {
            bool val = (row >> (varCount - v - 1)) & 1;
            table[row][v] = val;
        }
        results[row] = evaluate(expr, table[row]);
    }
}

// Display results
void showResults(string expr)
{
    cout << "\n"
         << CYAN << string(60, '=') << RESET << "\n";
    cout << CYAN << "LOGIC EXPRESSION: " << expr << RESET << "\n";
    cout << CYAN << string(60, '=') << RESET << "\n";

    cout << "\nVariables: ";
    for (int i = 0; i < varCount; i++)
    {
        cout << YELLOW << vars[i] << RESET;
        if (i < varCount - 1)
            cout << ", ";
    }

    int totalRows = 1;
    for (int i = 0; i < varCount; i++)
        totalRows *= 2;
    cout << " (" << totalRows << " combinations)\n\n";

    // Print header
    for (int i = 0; i < varCount; i++)
        cout << MAGENTA << vars[i] << "  " << RESET;
    cout << "| " << MAGENTA << "Result" << RESET << "\n";

    // Print separator
    cout << string(varCount * 3 + 8, '-') << "\n";

    for (int row = 0; row < totalRows; row++)
    {
        for (int v = 0; v < varCount; v++)
        {
            cout << (table[row][v] ? GREEN "T  " : RED "F  ") << RESET;
        }
        cout << "|   " << (results[row] ? GREEN "T" : RED "F") << RESET << "\n";
    }

    cout << string(varCount * 3 + 8, '-') << "\n";

    bool allTrue = true, allFalse = true;
    int trueCount = 0;

    for (int row = 0; row < totalRows; row++)
    {
        if (results[row])
        {
            trueCount++;
            allFalse = false;
        }
        else
        {
            allTrue = false;
        }
    }

    cout << "\n"
         << CYAN << "ANALYSIS:\n"
         << RESET;
    cout << string(30, '-') << "\n";

    if (allTrue)
        cout << GREEN << "TAUTOLOGY - Always true\n"
             << RESET;
    else if (allFalse)
        cout << RED << "CONTRADICTION - Always false\n"
             << RESET;
    else
        cout << YELLOW << "CONTINGENT - Depends on values\n"
             << RESET;

    cout << "\nTrue: " << GREEN << trueCount << "/" << totalRows << RESET;
    cout << " (" << GREEN << (trueCount * 100 / totalRows) << "%" << RESET << ")\n";
    cout << "False: " << RED << (totalRows - trueCount) << "/" << totalRows << RESET;
    cout << " (" << RED << ((totalRows - trueCount) * 100 / totalRows) << "%" << RESET << ")\n";
}

int main()
{
    system("cls");
    cout << BLUE << string(60, '=') << RESET << "\n";
    cout << BLUE << "DIRECT EVALUATION LOGIC SOLVER" << RESET << "\n";
    cout << BLUE << string(60, '=') << RESET << "\n\n";

    cout << YELLOW << "OPERATORS:\n"
         << RESET;
    cout << "  &  : AND     (p & q)\n";
    cout << "  |  : OR      (p | q)\n";
    cout << "  !  : NOT     (!p)\n";
    cout << "  >  : IMPLIES (p > q)\n\n";

    cout << YELLOW << "EXAMPLES:\n"
         << RESET;
    cout << "  1. p & q\n";
    cout << "  2. p | q\n";
    cout << "  3. !p\n";
    cout << "  4. p > q\n";
    cout << "  5. (p & q) | (!p & r)\n";
    cout << "  6. (a > b) & (b > c)\n\n";

    while (true)
    {
        string expr;
        cout << CYAN << "\nEnter expression (or 'quit'): " << RESET;
        getline(cin, expr);

        if (expr == "quit")
            break;
        if (expr.empty())
        {
            cout << RED << "Please enter an expression!\n"
                 << RESET;
            continue;
        }

        bool valid = false;
        for (char c : expr)
            if (c == '&' || c == '|' || c == '!' || c == '>')
                valid = true;

        if (!valid)
        {
            cout << RED << "Use operators: &, |, !, >\n"
                 << RESET;
            continue;
        }

        getVariables(expr);

        if (varCount == 0)
        {
            cout << RED << "No variables found! Use letters a-z\n"
                 << RESET;
            continue;
        }
        if (varCount > 6)
        {
            cout << RED << "Maximum 6 variables allowed\n"
                 << RESET;
            continue;
        }

        makeTruthTable(expr);
        showResults(expr);

        // Real example
        cout << "\n"
             << CYAN << string(60, '=') << RESET << "\n";
        cout << CYAN << "REAL EXAMPLE: Fraud Detection\n"
             << RESET;
        cout << CYAN << string(60, '=') << RESET << "\n";

        cout << "\nRule: (l & n) | (c & r)\n";
        cout << "Where: l=large, n=new, c=cross, r=risk\n";

        bool testVals[4] = {true, true, false, false}; // l=1, n=1, c=0, r=0
        char testVars[4] = {'l', 'n', 'c', 'r'};
        char oldVars[6];
        int oldCount = varCount;

        // Save current variables
        for (int i = 0; i < oldCount; i++)
            oldVars[i] = vars[i];

        // Set up test variables
        for (int i = 0; i < 4; i++)
            vars[i] = testVars[i];
        varCount = 4;

        bool testResult = evaluate("(l & n) | (c & r)", testVals);
        cout << "Test Case 1:\n";
        cout << "l=1 (large), n=1 (new), c=0, r=0\n";
        cout << "Fraud Alert: " << (testResult ? GREEN "YES\n" : RED "NO\n") << RESET;

        // Restore original variables
        varCount = oldCount;
        for (int i = 0; i < oldCount; i++)
            vars[i] = oldVars[i];

        cout << "\n"
             << CYAN << string(60, '=') << RESET << "\n";
    }

    cout << "\n"
         << BLUE << "Program ended. Thank you!\n"
         << RESET;

    return 0;
}