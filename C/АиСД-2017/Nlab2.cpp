#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <complex>
#include <string>
#include <stack>
#include <new>
#include <sstream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

struct MyValue {
    void *Value;
    bool Float;
    MyValue() : Value(NULL), Float() {} ;
    MyValue(void *val, bool bl) : Value(val), Float(bl) {};
    MyValue(MyValue *val) {
        if (val->Float) {
            float *fl = new float;
            *fl = *((float*)val->Value);
            Value = fl;
            Float = true;
        } else {
            Value = new complex<float>(*((complex<float>*)val->Value));
            Float = false;
        }
    }
    ~MyValue() {
        Value = NULL;
    }
    void Add(MyValue val) {
        if (Float) {
            if (val.Float) {
                *((float*)Value) += *((float*)val.Value);
            } else {
                Value = new complex<float>((*((complex<float>*)val.Value)) + (*((float*)Value)));
                Float = false;
            }
        } else {
            if (val.Float)
                *((complex<float>*)Value) += *((float*)val.Value); 
            else *((complex<float>*)Value) += *((complex<float>*)val.Value);
        }
    }
    void Subs(MyValue val) {
        if (Float) {
            if (val.Float) {
                *((float*)Value) -= *((float*)val.Value);
            } else {
                Value = new complex<float>((*((complex<float>*)val.Value)) - (*((float*)Value)));
                Float = false;
            }
        } else {
            if (val.Float)
                *((complex<float>*)Value) -= *((float*)val.Value); 
            else *((complex<float>*)Value) -= *((complex<float>*)val.Value);
        }
    }
    void MultToScalar(MyValue val) {
        if (Float) {
            if (val.Float) {
                *((float*)Value) *= *((float*)val.Value);
            } else {
                Value = new complex<float>((*((complex<float>*)val.Value)) * (*((float*)Value)));
                Float = false;
            }
        } else {
            if (val.Float)
                *((complex<float>*)Value) *= *((float*)val.Value); 
            else *((complex<float>*)Value) *= *((complex<float>*)val.Value);
        }
    }
    void DivByScalar(MyValue val) {
        if (Float) {
            if (val.Float) {
                *((float*)Value) /= *((float*)val.Value);
            } else {
                Value = new complex<float>((*((complex<float>*)val.Value)) / (*((float*)Value)));
                Float = false;
            }
        } else {
            if (val.Float)
                *((complex<float>*)Value) /= *((float*)val.Value); 
            else *((complex<float>*)Value) /= *((complex<float>*)val.Value);
        }
    }
    void Print() {
        cout << fixed;
        cout << setprecision(2); 
        if (Float)
            cout << *((float*)Value) << " ";
        else cout << "[" << ((complex<float>*)Value)->real() 
            << "+" << ((complex<float>*)Value)->imag() << "i] ";
    }
};

float ScanFloat(string str, int *i, char ch, int *ErrInd) {
    string strval;
    while (str[*i] != ch && *i < str.size()) {
        if (((int)str[*i] < 48 || (int)str[*i] > 57) && str[*i] != '.') {
            *ErrInd = 2;
            return -1;
        }
        strval += str[*i];
        *i = *i + 1;
    }
    if (str[*i] != ch || strval.empty() || *i == str.size()) {
        *ErrInd = 2;
        return -1;
    }
    float val;
    stringstream stream(strval);
    stream >> val;
    if (stream.fail()) {
        *ErrInd = 2;
        return -1;
    }
    return val;
}

MyValue *ScanMyValue(string str, int *i, bool neg, int *ErrInd) {
    if (str[*i] == '[') {
        *i = *i + 1;
        float real = ScanFloat(str, i, '+', ErrInd); 
        if (*ErrInd != 0 || str[*i] != '+') { 
            *ErrInd = 2;
            return NULL;
        }
        *i = *i + 1;
        float imag = ScanFloat(str, i, 'i', ErrInd);
        if (*ErrInd != 0 || str[*i] != 'i' || str[*i + 1] != ']') {
            *ErrInd = 2;
            return NULL;
        }
        *i = *i + 2;
        if (neg)
            real *= -1;
        return new MyValue(new complex<float>(real, imag), false);
    } else if (((int)str[*i] > 47 && (int)str[*i] < 58) || str[*i] == '-') {
        float *val = new float;
        *val = ScanFloat(str, i, ' ', ErrInd);
        if (*ErrInd != 0 || str[*i] != ' ') {
            *ErrInd = 1;
            return NULL;
        }
        if (neg)
            *val *= -1;
        return new MyValue(val, true);
    } else {
        *ErrInd = 2;
        return NULL;
    }
}

MyValue *UsersMyValue() {    
    cout << " ______________________________________________________ \n"
        "|                                                      |\n"
        "| Write a scalar.                                      |\n" 
        "| Scalar could be a complex number in the form [x+yi]. |\n"
        "| For example, [1+1i] or 1.1.                          |\n"
        "|______________________________________________________|\n\n";
    MyValue *val;
    string str;
    while (true) {
        cout << "Please enter a valid scalar: ";
        getline(cin, str);
        str += ' ';
        int ErrInd = 0, i = 0;
        val = ScanMyValue(str, &i, false, &ErrInd);
        if (!ErrInd)
            break;
        if (val != NULL)
            val->~MyValue();
        cout << "Scalar is not valid!\n"
            "Scalar should be in the form [1+1i] or just real number.\n";
    }
    return val;
}

MyValue *RandomMyValue() {
    bool bl = rand() % 2;
    if (bl) {
        float *val = new float;
        *val = (rand() % 201 - 100) * 22.0 / 23;
        return new MyValue(val, bl); 
    } else {
        float real = (rand() % 201 - 100) * 22.0 / 23,
              imag = (rand() % 201 - 100) * 22.0 / 23;
        return new MyValue(new complex<float>(real, imag), bl);
    }
}

struct Node {
    MyValue *Coeficient;
    MyValue *Argument;
    int N;
    Node *Next;
    Node() : Coeficient(NULL), Argument(NULL), N(0), Next(NULL) {};
    Node(MyValue *coeficient, MyValue *argument, int n) {
        Coeficient = coeficient;
        Argument = argument;
        N = n;
        Next = NULL;
    }
    ~Node() {
        if (Coeficient != NULL)
            Coeficient->~MyValue();
        if (Argument != NULL)
            Argument->~MyValue();
    }
    void Add(Node node) {
        if(Coeficient != NULL && node.Coeficient != NULL)
            Coeficient->Add(*node.Coeficient);
        if(Argument != NULL && node.Argument != NULL)
            Argument->Add(*node.Argument);
    }
    void Subs(Node node) {
        if(Coeficient != NULL && node.Coeficient != NULL)
            Coeficient->Subs(*node.Coeficient);
        if(Argument != NULL && node.Argument != NULL)
            Argument->Subs(*node.Argument);
    }
    void MultToScalar(MyValue x) {
        if(Coeficient != NULL)
            Coeficient->MultToScalar(x);
        if(Argument != NULL)
            Argument->MultToScalar(x);
    }
    void Print() {
        if (Coeficient != NULL) {
            cout << "+ ";
            Coeficient->Print();
        }
        if (Argument != NULL) {
            cout << "* ";
            Argument->Print();
        } else {
            cout << "* x" << N << " ";
        }
    }
};

class LinearForm {
    int N;
    MyValue *FreeTerm;
    Node *HeadTerm, *TailTerm;
    public:
    LinearForm() : N(0), FreeTerm(NULL), HeadTerm(NULL), TailTerm(NULL) {};
    ~LinearForm() {
        N = 0;
        if (FreeTerm != NULL)
            FreeTerm->~MyValue();
        while (HeadTerm != TailTerm) {
            Node *temp = HeadTerm;
            HeadTerm = HeadTerm->Next;
            if (temp != NULL)
                temp->~Node();
        }
    }
    bool Valid();
    void PushFreeTerm(MyValue *freeterm) {
        FreeTerm = freeterm;
    }
    void PushTerm(MyValue *coeficient, MyValue *argument);
    void AddLinearForm(LinearForm lf);
    void SubsLinearForm(LinearForm lf);
    void MultToScalarLinearForm(MyValue x);
    MyValue SolveLinearForm(bool rand);
    void DeleteArguments();
    void Print();
};
bool LinearForm::Valid() {
    if (FreeTerm != NULL && N > 0)
        return true;
    else return false; 
}
void LinearForm::PushTerm(MyValue *coeficient, MyValue *argument) {
    N++;
    Node *temp = new Node(coeficient, argument, N);
    if (TailTerm != NULL) {
        TailTerm->Next = temp;
        TailTerm = temp;
    } else {
        HeadTerm = TailTerm = temp;
    }
}
void LinearForm::AddLinearForm(LinearForm lf) {
    FreeTerm->Add(*lf.FreeTerm);
    Node *temp = HeadTerm;
    Node *temp1 = lf.HeadTerm;
    while (temp != NULL && temp1 != NULL) {
        temp->Add(*temp1);
        temp = temp->Next;
        temp1 = temp1->Next;
    }
    cout << "Successfully added!\n";
}
void LinearForm::SubsLinearForm(LinearForm lf) {
    FreeTerm->Subs(*lf.FreeTerm);
    Node *temp = HeadTerm;
    Node *temp1 = lf.HeadTerm;
    while (temp != NULL && temp1 != NULL) {
        temp->Subs(*temp1);
        temp = temp->Next;
        temp1 = temp1->Next;
    }
    cout << "Successfully substracted!\n";
}
void LinearForm::MultToScalarLinearForm(MyValue x) {
    FreeTerm->MultToScalar(x);
    Node *temp = HeadTerm;
    while (temp != NULL) {
        temp->MultToScalar(x);
        temp = temp->Next;
    }
    cout << "Successfully multiplied!\n";
}

MyValue *UsersArgument(bool *bl, int n) {
    //MyValue *val = new MyValue();
    MyValue *val;
    string str, Err[3];
    Err[0] = "Argument accepted.";
    Err[1] = "Only one undefined argument is allowed.";
    Err[2] = "Argument is not valid!\n"
            "Argument should be in the form [1+1i] or just real number.";
    while (true) {
        cout << "Please enter a valid argument: ";
        cout << "x" << n << " = ";
        getline(cin, str);
        int ErrInd = 0, i = 0;
        if (str[0] == '?') {
            if (!*bl) {
                *bl = true;
                return NULL;
            }
            ErrInd = 1;
        } else {
            str += ' ';
            val = ScanMyValue(str, &i, false, &ErrInd);
        }
        cout << Err[ErrInd] << "\n";
        if (!ErrInd)
            break;
        if (val != NULL)
            val->~MyValue();
    }
    return val;
}
MyValue *RandomArgument(bool *bl) {
    if (!*bl) {
        *bl = rand() % 2;
        if (!*bl)
            return RandomMyValue();
        else return NULL;
    }
    return RandomMyValue();
}

MyValue LinearForm::SolveLinearForm(bool rand) {
    if (!rand)
        cout << " ________________________________________________________ \n"
            "|                                                        |\n"
            "| Write an argument.                                     |\n" 
            "| Argument could be a complex number in the form [x+yi]. |\n"
            "| For example, [1+1i] or 1.1.                            |\n"
            "| Or sign '?' if you want to find this argument.         |\n"
            "|________________________________________________________|\n\n";
    bool bl = false;
    MyValue x(FreeTerm), xx;
    Node *temp = HeadTerm;
    while (temp != NULL) {
        if (temp->Next == NULL && !bl) {
            xx = *temp->Coeficient;
            break;
        }
        if (rand) {
            temp->Argument = RandomArgument(&bl);
            if (temp->Argument == NULL) {
                cout << "x" << temp->N << " = ?\n";
            } else {
                cout << "x" << temp->N << " = ";
                temp->Argument->Print();
                cout << "\n";
            }
        } else {
            temp->Argument = UsersArgument(&bl, temp->N);
        }
        if (temp->Argument != NULL) {
            MyValue val(temp->Argument);
            val.MultToScalar(*temp->Coeficient);
            x.Add(val);
        } else {
            xx = *temp->Coeficient;
        }
        temp = temp->Next;
    }
    float xxx = -1;
    x.DivByScalar(xx);
    x.MultToScalar(MyValue(&xxx, true));
    cout << "Successfully solved!\n";
    return x;
}
void LinearForm::DeleteArguments() {
    Node *temp = HeadTerm;
    while (temp != NULL) {
        temp->Argument = NULL;
        temp = temp->Next;
    }
}
void LinearForm::Print() {
    cout << "Linear Form of " << N << " variable:\n";
    if (FreeTerm != NULL)
        FreeTerm->Print();
    Node *temp = HeadTerm;
    while (temp != NULL) {
        temp->Print();
        temp = temp->Next;
    }
}

LinearForm *ScanLinearForm(string str, int *ErrInd) {
    LinearForm *lf = new LinearForm();
    bool ft = false, neg = false;
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == ' ' || str[i] == '+' || str[i] == '*') {
            continue;
        } else if (str[i] == '-' && !neg) {
            neg = true;
            continue;
        } else if (str[i] == '-' && neg) {
            neg = false;
            continue;
        } else if ((str[i] == 'x' || str[i] == 'X') 
                && i + 1 != str.size() 
                && (int)str[i + 1] > 47 && (int)str[i + 1] < 58) {
            i++;
            continue;
        } else {
            if (!ft) {
                lf->PushFreeTerm(ScanMyValue(str, &i, neg, ErrInd));
                ft = true;
            } else {
                lf->PushTerm(ScanMyValue(str, &i, neg, ErrInd), NULL);
            }
            neg = false;
        }
    }
    if (*ErrInd != 0) {
        lf->~LinearForm();
        return NULL;
    } else {
        return lf;
    }
}

bool ValidString(string str) {
    stack<char> st;
    int i, strlen = str.length();
    for (i = 0; i < strlen; i++) {
        if ((int)str[i] > 57 || (int)str[i] < 48)
            if (str[i] != '+' && str[i] != '-' && str[i] != '*' && str[i] != '.' 
                    && str[i] != 'x' && str[i] != 'X' && str[i] != 'i' && str[i] != ' ' 
                    && str[i] != '[' && str[i] != ']' && str[i] != '(' && str[i] != ')')
                return false;
        if (str[i] == '[') {
            st.push(str[i]);
            continue;
        }
        if (str[i] == ']') {
            if (st.top() == '[')
                st.pop();
            else break;
            continue;
        }
    }
    if (st.empty() && i == strlen)
        return true;
    return false;
}

LinearForm *UsersLinearForm() {
    cout << " _______________________________________________________________ \n"
        "|                                                               |\n"
        "| Write a linear form in the form A0 + A1 * X1 + ... + An * Xn. |\n" 
        "| Where A is Coeficient and X is Argument.                      |\n"
        "| Coeficients could be a complex number in the form [x+yi].     |\n"
        "| For example, 1.1 + 2.3 * x1 + 32 * x2 + [1+1i] * x3.          |\n"
        "|_______________________________________________________________|\n\n";
    string str, Err[3];
    LinearForm *lf;
    Err[0] = "Linear form accepted.";
    Err[1] = "Linear form is not valid!\n"
        "Linear form shoudl be in the form A0 + A1 * X1 + ... + An * Xn.";
    Err[2] = "Coeficient is not valid!\n"
        "Coeficient should be in the form [x+yi] or just real number.";
    while (true) {
        cout << "Please enter a valid linear form: ";
        getline(cin, str);
        int ErrInd = 0;
        if (ValidString(str)) {
            lf = ScanLinearForm(str, &ErrInd);
            if (!ErrInd && !lf->Valid()) {
                ErrInd = 1;
            } else if (!ErrInd) {
                cout << "Did you mean?\n";
                lf->Print();
                cout << "\n";
                while (true) {
                    int command;
                    cout << "Please enter a valid command (1/0): ";
                    cin >> command;
                    if (command == 1 || command == 0) {
                        if (command == 0)
                            ErrInd = 1;
                        break;
                    }
                }
            }
        } else {
            ErrInd = 1;
        }
        cout << Err[ErrInd] << "\n";
        if (!ErrInd)
            break;
    }
    return lf;
}

LinearForm *RandomLinearForm() {
    while (true) {
        LinearForm *lf = new LinearForm();
        int sz = rand() % 9 + 2;
        cout << sz << "\n";
        lf->PushFreeTerm(RandomMyValue());
        for (int i = 0; i < sz; i++)
            lf->PushTerm(RandomMyValue(), NULL);
        cout << "Do you like\n"; 
        lf->Print();
        cout << "?\n";
        while (true) {
            int command;
            cout << "Please enter a valid command (1/0): ";
            cin >> command;
            if (command == 1)
                return lf;
            if (command == 0) {
                lf->~LinearForm();
                break;
            }
        }
    }
}

bool GetRandomArguments() {
    cout << "Program needs arguments.\nDo you want to use random arguments?\n";
    while (true) {
        int command;
        cout << "Please enter a valid command (1/0): ";
        cin >> command;
        if (command == 1)
            return true;
        else if (command == 0)
            return false;
    }
}

MyValue *GetMyValue() {
    cout << "Program needs a scalar.\nDo you want to use random scalar?\n";
    MyValue *val;
    while (true) {
        int command;
        cout << "Please enter a valid command (1/0): ";
        cin >> command;
        if (command == 1) {
            val = RandomMyValue();
            val->Print();
            cout << "\n";
            break;
        } else if (command == 0) {
            val = UsersMyValue();
            break;
        }
    }
    return val;
}

LinearForm *GetLinearForm() {
    cout << "Program needs a linear form.\nDo you want to use random linear form?\n";
    LinearForm *lf;
    while (true) {
        int command;
        cout << "Please enter a valid command (1/0): ";
        cin >> command;
        if (command == 1) {
            lf = RandomLinearForm();
            lf->Print();
            cout << "\n";
            break;
        } else if (command == 0) {
            lf = UsersLinearForm();
            break;
        }
    }
    return lf;
}

void StartProgram() {
    LinearForm *lf = GetLinearForm();
    while (true) {
        cout << " _____________________________________________________________________ \n"
            "|                                                                     |\n"
            "| Enter 1 if you want to add some linear form.                        |\n"
            "| Enter 2 if you want to substract some linear form.                  |\n"
            "| Enter 3 if you want to multiply by some scalar.                     |\n"
            "| Enter 4 if you want to solve your linear form with given arguments. |\n"
            "| Enter 5 if you want to print your linear form.                      |\n"
            "| Enter 0 if you want to stop program.                                |\n"
            "|_____________________________________________________________________|\n\n";
        int command;
        cout << "Please enter a valid command: ";
        cin >> command;
        if (command == 0) {
            break;
        } else if (command == 1) {
            LinearForm *lf1 = GetLinearForm();
            lf->AddLinearForm(*lf1);
            lf1->~LinearForm();
        } else if (command == 2) {
            LinearForm *lf1 = GetLinearForm();
            lf->SubsLinearForm(*lf1);
            lf1->~LinearForm();
        } else if (command == 3) {
            MyValue *val = GetMyValue();
            lf->MultToScalarLinearForm(*val);
            val->~MyValue();
        } else if (command == 4) {
            lf->Print();
            cout << "\n";
            MyValue x;
            if (GetRandomArguments())
                x = lf->SolveLinearForm(true);
            else x = lf->SolveLinearForm(false);
            cout << "Solution of undefined argument is ";
            x.Print();
            cout << "\n";
            x.~MyValue();
            lf->DeleteArguments();
        } else if (command == 5) {
            lf->Print();
            cout << "\n";
        }
    }
}

void TestProgram() {
}

int main() {
    srand(time(NULL));
    while (true) {
        cout << " _____________________________________________________ \n"
            "|                                                     |\n"
            "| This program can add, substract, multiply by scalar |\n" 
            "| and solve linear forms with given arguments.        |\n"
            "|                                                     |\n"
            "| Enter 1 if you want to start program.               |\n"
            "| Enter 2 if you want to test program.                |\n"
            "| Enter 0 if you want to switch of program.           |\n"
            "|_____________________________________________________|\n\n";
        int command;
        cout << "Please enter a valid command: ";
        cin >> command;
        if (command == 1)
            StartProgram();
        else if (command == 2)
            TestProgram();
        else break;
    }

	return 0;
}
