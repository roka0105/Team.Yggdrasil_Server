#pragma once
namespace Core
{
    template<typename T>
    class get
    {
    private:
        const T& m_data;
    public:
        get(const T& data) :m_data(data){}
        get(const get<T>& get) :m_data(get.m_data) {}
        operator const T& ()const
        {
            return m_data;
        }
        const T& operator=(const T& data);
    private:
        get<T>& operator=(const get<T>&);
    };
    template<typename T>
    class set
    {
    public:
    private:
    };
    template<typename T>
    class all
    {
    public:
    private:
    };
    template <typename T, template<typename T> class ProperyPolicy = all>
    class Property
    {
    public:
        Property();
    private:
    };

}
void test()
{
    float f1 = 0.2;
    Core::Property<float, Core::all> a;
    Core::get<float> f(f1);
    const float& x = f;
    float y;
    y = f;

}

