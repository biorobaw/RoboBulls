#ifndef STRATEGY_H
#define STRATEGY_H

class Strategy
{
public:
    Strategy();

    virtual void assignBeh() = 0;

    virtual ~Strategy(){}

private:
};

#endif // STRATEGY_H
