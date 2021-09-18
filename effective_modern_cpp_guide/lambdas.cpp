#include <vector>
#include <functional>
#include <memory>

void avoid_default_capture_modes()
{
    // unikaj używania w lambdach & oraz = 
    // jawnie mów co ma zostać przechwycone
    //Default by-reference capture can lead to dangling references.
    // Default by-value capture is susceptible to dangling pointers (especially this),
    // and it misleadingly suggests that lambdas are self-contained.

    //załóżmy ,że mamy taką klasę

    class Widget 
    {
    public:
        void addFilter() const;
    private:
        int divisor;
    };
    
    // jak poprawnie zaimplementować lambdę w tej funkcji?

    //a no tak
    void Widget::addFilter() const
    {
        filters.emplace_back(
            [divisor = divisor](int value)// jawne przechwycenie przez kopię
            { 
                return value % divisor == 0; 
            }
        );
    } 
}

void use_init_capture_to_move_objects()
{
    class Widget 
    {
    public:
        bool isValidated() const;
        bool isProcessed() const;
        bool isArchived() const;
    private:
    };

    int data;

    auto func = [data = std::move(data)]
    { /* uses of data */ };

    //w c++11 nie można użyć std::move do przenoszenia do lambd
    //ale mozna to zasymulować poprzez std::bind

    auto func = std::bind(
        [](const std::unique_ptr<Widget>& pw)
        { 
            return pw->isValidated() && pw->isArchived(); 
        },
        std::make_unique<Widget>()
    );

}

void use_lambda_instead_bind()
{
    // In C++11 only, std::bind may be useful for implementing move capture or
    // for binding objects with templatized function call operators.

    //bindy są mniej efektywne i mniej czytelne 
}

int main()
{

}