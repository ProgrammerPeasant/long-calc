#include "InfFloat.h"

int InfFloat::CompareNum(const InfFloat &left, const InfFloat &right)
{
    if ((left.m_number.size() - left.m_decimals) > (right.m_number.size() - right.m_decimals))
        return 1;
    else if ((left.m_number.size() - left.m_decimals) < (right.m_number.size() - right.m_decimals))
        return 2;

    if (left.m_decimals > right.m_decimals)
    {
        InfFloat tmp;
        tmp = right;
        while (left.m_decimals > tmp.m_decimals)
        {
            tmp.m_decimals++;
            tmp.m_number.push_front('0');
        }

        for (int i = left.m_number.size() - 1; i >= 0; i--)
        {
            if (left.m_number[i] > tmp.m_number[i])
                return 1;
            else if (left.m_number[i] < tmp.m_number[i])
                return 2;
        }
        return 0;
    }
    else if (left.m_decimals < right.m_decimals)
    {
        InfFloat tmp;
        tmp = left;
        while (tmp.m_decimals < right.m_decimals)
        {
            tmp.m_decimals++;
            tmp.m_number.push_front('0');
        }

        for (int i = tmp.m_number.size() - 1; i >= 0; i--)
        {
            if (tmp.m_number[i] > right.m_number[i])
                return 1;
            else if (tmp.m_number[i] < right.m_number[i])
                return 2;
        }
        return 0;
    }
    else
    {
        for (int i = left.m_number.size() - 1; i >= 0; i--)
        {
            if (left.m_number[i] > right.m_number[i])
                return 1;
            else if (left.m_number[i] < right.m_number[i])
                return 2;
        }
        return 0;
    }

    return 0;
};

InfFloat InfFloat::Sum(const InfFloat &left, const InfFloat &right)
{
    InfFloat tmp;
    int carry = 0;
    unsigned long long int loopsize = (left.m_number.size() > right.m_number.size()) ? left.m_number.size() : right.m_number.size();

    for (unsigned long long int i = 0; i < loopsize; ++i)
    {
        int val1, val2;
        val1 = (i > left.m_number.size() - 1) ? 0 : CharToInt(left.m_number[i]);
        val2 = (i > right.m_number.size() - 1) ? 0 : CharToInt(right.m_number[i]);

        int aus = val1 + val2 + carry;

        carry = 0;
        if (aus > 9)
        {
            carry = 1;
            aus = aus - 10;
        }

        tmp.m_number.push_back(IntToChar(aus));
    }
    if (carry != 0)
        tmp.m_number.push_back(IntToChar(carry));

    return tmp;
};

InfFloat InfFloat::Subtract(const InfFloat &left, const InfFloat &right)
{
    InfFloat tmp;
    int carry = 0;
    int aus;

    for (unsigned long long int i = 0; i < left.m_number.size(); ++i)
    {
        int val1, val2;
        val1 = CharToInt(left.m_number[i]);
        val2 = (i > right.m_number.size() - 1) ? 0 : CharToInt(right.m_number[i]);

        val1 -= carry;

        if (val1 < val2)
        {
            aus = 10 + val1 - val2;
            carry = 1;
        }
        else
        {
            aus = val1 - val2;
            carry = 0;
        }

        tmp.m_number.push_back(IntToChar(aus));
    }

    return tmp;
};

InfFloat InfFloat::Multiply(const InfFloat &left, const InfFloat &right)
{
    InfFloat ris;
    InfFloat tmp;
    ris.m_number.push_back('0');
    int carry = 0;

    for (unsigned long long int i = 0; i < right.m_number.size(); ++i)
    {
        for (unsigned long long int k = 0; k < i; ++k)
        {
            tmp.m_number.push_front('0');
        }

        for (char j : left.m_number)
        {
            int aus = CharToInt(right.m_number[i]) * CharToInt(j) + carry;
            carry = 0;
            if (aus > 9)
            {
                while (aus > 9)
                {
                    carry++;
                    aus -= 10;
                }
            }
            tmp.m_number.push_back(IntToChar(aus));
        }

        if (carry != 0)
            tmp.m_number.push_back(IntToChar(carry));
        carry = 0;

        ris = Sum(ris, tmp);
        tmp.m_number.clear();
    }

    return ris;
};

InfFloat InfFloat::Abs() const
{
    auto res = InfFloat(*this);
    res.m_sign = '+';
    return res;
}

InfFloat operator""_inf(const char* str, size_t size) {
    return InfFloat(std::string(str, size));
}

std::string InfFloat::PeriodicToCommon(const std::string &str)
{
    std::stringstream ss;
    std::stringstream per;
    bool periodic = false;
    for (char i : str)
    {
        if (i == '+')
        {
            ss << '+';
        }
        else if (i == '-')
        {
            ss << '-';
        }
        if (i == '(')
        {
            periodic = true;
        }
        else if (i == ')')
        {
            break;
        }
        if (isdigit(i) || i == '.')
        {
            if (!periodic)
            {
                ss << i;
            }
            else
            {
                per << i;
            }
        }
    }
    if (periodic)
    {
        unsigned long long int count = 20 / per.str().size();
        if (count == 0)
            count++;
        for (unsigned long long int i = 0; i < count; i++)
            ss << per.str();
    }
    return ss.str();
}

InfFloat &InfFloat::operator=(int Num)
{
    *this = std::to_string(Num);
    return *this;
};

InfFloat &InfFloat::operator=(double Num)
{
    *this = std::to_string(Num);
    return *this;
};

InfFloat &InfFloat::operator=(const std::string &strNum)
{
    m_number.clear();
    m_decimals = 0;
    m_sign = '\0';

    int count = 0;
    if (strNum[count] == '+')
    {
        m_sign = '+';
        count++;
    }
    else if (strNum[count] == '-')
    {
        m_sign = '-';
        count++;
    }
    else if (isdigit(strNum[count]))
        m_sign = '+';
    else
    {
        return *this;
    }

    bool start_dec = false;
    while (strNum[count] != '\0')
    {
        if (!start_dec)
            if (strNum[count] == '.')
            {
                if (m_number.empty())
                {
                    m_sign = '\0';
                    return *this;
                }
                start_dec = true;
                count++;
            }

        if (isdigit(strNum[count]))
        {
            m_number.push_front(strNum[count]);
            count++;
            if (start_dec)
                m_decimals++;
        }
        else
        {
            m_sign = '\0';
            m_number.clear();
            m_decimals = 0;
            return *this;
        }
    }
    this->LeadZeroes();
    return *this;
};

InfFloat::InfFloat(const std::string &m_number)
{
    *this = PeriodicToCommon(m_number);
}

InfFloat::InfFloat(const int m_number)
{
    *this = m_number;
}

InfFloat::InfFloat(const double m_number)
{
    *this = m_number;
}

InfFloat::InfFloat()
{
    m_sign = '\0';
    m_decimals = 0;
}

InfFloat InfFloat::operator+(const InfFloat &other) const
{
    InfFloat tmp;
    InfFloat left, right;
    left = *this;
    right = other;

    if (left.m_decimals > right.m_decimals)
        while (left.m_decimals > right.m_decimals)
        {
            right.m_decimals++;
            right.m_number.push_front('0');
        }
    else if (left.m_decimals < right.m_decimals)
        while (left.m_decimals < right.m_decimals)
        {
            left.m_decimals++;
            left.m_number.push_front('0');
        }

    if ((left.m_sign == '+') && (right.m_sign == '-'))
    {
        int check = InfFloat::CompareNum(left, right);
        if (check == 0)
        {
            tmp = 0;
            return tmp;
        }
        if (check == 1)
        {
            tmp = InfFloat::Subtract(left, right);
            tmp.m_sign = '+';
            tmp.m_decimals = left.m_decimals;
            tmp.LeadZeroes();
            return tmp;
        }
        if (check == 2)
        {
            tmp = InfFloat::Subtract(right, left);
            tmp.m_sign = '-';
            tmp.m_decimals = left.m_decimals;
            tmp.LeadZeroes();
            return tmp;
        }
    }
    if ((left.m_sign == '-') && (right.m_sign == '+'))
    {
        int check = InfFloat::CompareNum(left, right);
        if (check == 0)
        {
            tmp = 0;
            return tmp;
        }
        if (check == 1)
        {
            tmp = InfFloat::Subtract(left, right);
            tmp.m_sign = '-';
            tmp.m_decimals = left.m_decimals;
            tmp.LeadZeroes();
            return tmp;
        }
        if (check == 2)
        {
            tmp = InfFloat::Subtract(right, left);
            tmp.m_sign = '+';
            tmp.m_decimals = left.m_decimals;
            tmp.LeadZeroes();
            return tmp;
        }
    }

    if ((left.m_sign == '+') && (right.m_sign == '+'))
    {
        tmp = InfFloat::Sum(left, right);
        tmp.m_sign = '+';
        tmp.m_decimals = left.m_decimals;
        return tmp;
    }
    if ((left.m_sign == '-') && (right.m_sign == '-'))
    {
        tmp = InfFloat::Sum(left, right);
        tmp.m_sign = '-';
        tmp.m_decimals = left.m_decimals;
        return tmp;
    }

    return 0;
};

InfFloat InfFloat::operator-(const InfFloat &other) const
{
    InfFloat tmp;

    InfFloat left, right;
    left = *this;
    right = other;

    if (left.m_decimals > right.m_decimals)
        while (left.m_decimals > right.m_decimals)
        {
            right.m_decimals++;
            right.m_number.push_front('0');
        }
    else if (left.m_decimals < right.m_decimals)
        while (left.m_decimals < right.m_decimals)
        {
            left.m_decimals++;
            left.m_number.push_front('0');
        }

    if ((left.m_sign == '+') && (right.m_sign == '-'))
    {
        tmp = InfFloat::Sum(left, right);
        tmp.m_sign = '+';
        tmp.m_decimals = left.m_decimals;
        return tmp;
    }
    if ((left.m_sign == '-') && (right.m_sign == '+'))
    {
        tmp = InfFloat::Sum(left, right);
        tmp.m_sign = '-';
        tmp.m_decimals = left.m_decimals;
        return tmp;
    }

    if ((left.m_sign == '+') && (right.m_sign == '+'))
    {
        int check = InfFloat::CompareNum(left, right);
        if (check == 0)
        {
            tmp = 0;
            return tmp;
        }
        if (check == 1)
        {
            tmp = InfFloat::Subtract(left, right);
            tmp.m_sign = '+';
            tmp.m_decimals = left.m_decimals;
            tmp.LeadZeroes();
            return tmp;
        }
        if (check == 2)
        {
            tmp = InfFloat::Subtract(right, left);
            tmp.m_sign = '-';
            tmp.m_decimals = left.m_decimals;
            tmp.LeadZeroes();
            return tmp;
        }
    }

    if ((left.m_sign == '-') && (right.m_sign == '-'))
    {
        int check = InfFloat::CompareNum(left, right);
        if (check == 0)
        {
            tmp = 0;
            return tmp;
        }
        if (check == 1)
        {
            tmp = InfFloat::Subtract(left, right);
            tmp.m_sign = '-';
            tmp.m_decimals = left.m_decimals;
            tmp.LeadZeroes();
            return tmp;
        }
        if (check == 2)
        {
            tmp = InfFloat::Subtract(right, left);
            tmp.m_sign = '+';
            tmp.m_decimals = left.m_decimals;
            tmp.LeadZeroes();
            return tmp;
        }
    }

    return -1;
};

InfFloat InfFloat::operator*(const InfFloat &other) const
{
    InfFloat tmp;

    tmp = InfFloat::Multiply(*this, other);
    if (((other.m_sign == '-') && (this->m_sign == '-')) || ((other.m_sign == '+') && (this->m_sign == '+')))
        tmp.m_sign = '+';
    else
        tmp.m_sign = '-';

    tmp.m_decimals = other.m_decimals + this->m_decimals;
    tmp.LeadZeroes();

    return tmp;
};

InfFloat InfFloat::operator/(const InfFloat &other) const
{
    InfFloat tmp;

    InfFloat quotient, remainder, divisor, dividend, zero;
    zero = 0;

    if (other == zero)
    {
        return tmp;
    }

    dividend = (*this > zero) ? (*this) : (*this * (-1));
    divisor = (other > zero) ? (other) : (other * (-1));
    remainder.m_sign = '+';

    while ((dividend.m_decimals != 0) || (divisor.m_decimals != 0))
    {
        if (dividend.m_decimals == 0)
            dividend.m_number.push_front('0');
        else
            dividend.m_decimals--;

        if (divisor.m_decimals == 0)
            divisor.m_number.push_front('0');
        else
            divisor.m_decimals--;
    }
    dividend.LeadZeroes();
    divisor.LeadZeroes();

    int div_precision = (this->m_decimals > other.m_decimals) ? (this->m_decimals) : (other.m_decimals);
    for (int i = 0; i < div_precision; i++)
        dividend.m_number.push_front('0');

    int check = InfFloat::CompareNum(dividend, divisor);

    if (check == 0)
    {
        tmp.m_number.push_front('1');
    }
    if (check == 2)
    {
        return zero;
    }
    else
    {
        while (!dividend.m_number.empty())
        {
            remainder.m_number.push_front(*(dividend.m_number.rbegin()));
            dividend.m_number.pop_back();

            bool is_zero = true;
            std::deque<char>::const_iterator zero_iter = remainder.m_number.begin();
            for (; zero_iter != remainder.m_number.end(); ++zero_iter)
                if (*zero_iter != '0')
                    is_zero = false;

            if ((remainder >= divisor) && (!is_zero))
            {
                int Q_sub = 0;
                int min = 0;
                int max = 9;

                while (remainder >= divisor)
                {
                    int avg = max - min;
                    int mod_avg = avg / 2;
                    avg = (avg - mod_avg * 2) ? (mod_avg + 1) : (mod_avg);

                    int div_check = InfFloat::CompareNum(remainder, divisor * avg);

                    if (div_check == 2)
                    {
                        max = avg;
                    }
                    else
                    {
                        Q_sub = Q_sub + avg;
                        remainder = remainder - divisor * avg;

                        max = 9;
                    }
                }

                quotient.m_number.push_front(InfFloat::IntToChar(Q_sub));

                bool is_zero = true;
                std::deque<char>::const_iterator zero_iter = remainder.m_number.begin();
                for (; zero_iter != remainder.m_number.end(); ++zero_iter)
                    if (*zero_iter != '0')
                        is_zero = false;
                if (is_zero)
                    remainder.m_number.clear();
            }
            else
            {
                quotient.m_number.push_front('0');
            }
        }
        tmp = quotient;
    }

    if (((this->m_sign == '-') && (other.m_sign == '-')) || ((this->m_sign == '+') && (other.m_sign == '+')))
        tmp.m_sign = '+';
    else
        tmp.m_sign = '-';

    tmp.m_decimals = div_precision;
    tmp.LeadZeroes();

    return tmp;
};

bool InfFloat::operator==(const InfFloat &right) const
{
    int check = CompareNum(*this, right);
    if ((check == 0) && (m_sign == right.m_sign))
        return true;
    return false;
};

bool InfFloat::operator!=(const InfFloat &right) const
{
    return !(*this == right);
};

bool InfFloat::operator>(const InfFloat &right) const
{
    if (((m_sign == '+') && (right.m_sign == '+')))
    {
        int check = CompareNum(*this, right);
        if (check == 1)
            return true;
        return false;
    }
    if (((m_sign == '-') && (right.m_sign == '-')))
    {
        int check = CompareNum(*this, right);
        if (check == 2)
            return true;
        return false;
    }
    if (((m_sign == '-') && (right.m_sign == '+')))
        return false;
    if (((m_sign == '+') && (right.m_sign == '-')))
        return true;

    return -1;
};

bool InfFloat::operator>=(const InfFloat &right) const
{
    return !(*this < right);
};

bool InfFloat::operator<(const InfFloat &right) const
{
    if (((m_sign == '+') && (right.m_sign == '+')))
    {
        int check = CompareNum(*this, right);
        if (check == 2)
            return true;
        return false;
    }
    if (((m_sign == '-') && (right.m_sign == '-')))
    {
        int check = CompareNum(*this, right);
        if (check == 1)
            return true;
        return false;
    }
    if (((m_sign == '-') && (right.m_sign == '+')))
        return true;
    if (((m_sign == '+') && (right.m_sign == '-')))
        return false;

    return -1;
};

bool InfFloat::operator<=(const InfFloat &right) const
{
    return !(*this > right);
};

std::ostream &operator<<(std::ostream &out, const InfFloat &right)
{
    if (right.m_sign == '-')
        out << "-";
    for (int i = right.m_number.size() - 1; i >= 0; --i)
    {
        out << right.m_number[i];
        if ((i == right.m_decimals) && (i != 0))
            out << '.';
    }

    return out;
};
std::string InfFloat::ToString() const
{
    std::string var;
    std::stringstream ss;
    if (m_sign == '-')
    {
        ss << m_sign;
    }
    for (int i = m_number.size() - 1; i >= 0; i--)
    {
        ss << m_number[i];
        if ((i == m_decimals) && (i != 0))
            ss << ".";
    }
    ss >> var;
    return var;
};

void InfFloat::SetPrecision(int prec)
{
    if (this->m_decimals < prec)
    {
        while (this->m_decimals < prec)
        {
            this->m_decimals = this->m_decimals + 1;
            this->m_number.push_front('0');
        }
    }
    else if (this->m_decimals > prec)
    {
        char last;
        while (this->m_decimals > prec)
        {
            last = m_number[0];
            this->m_decimals = this->m_decimals - 1;
            this->m_number.pop_front();
        }
        if (CharToInt(last) >= 5)
        {
            if (prec == 0)
                (*this) + 1;
            else
            {
                std::stringstream ss;
                ss << "0.";
                for (int i = 1; i < prec; i++)
                    ss << "0";
                ss << "1";
                InfFloat aus(ss.str());
                *this = *this + aus;
            }
        }
    }
};

void InfFloat::LeadZeroes()
{
    for (int i = m_number.size() - 1; i > m_decimals; --i)
    {
        if (m_number[i] == '0')
            m_number.pop_back();
        else
            break;
    }
};
