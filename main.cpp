#include <iostream>
#include <functional>
#include <utility>
#include <array>
#include <string>

#define DEBUG

#ifdef DEBUG
#define DEBUG_MSG(str) std::cout << str << std::endl;
#define DEBUG_MSGW(str) std::cout << str;
#else
#define DEBUG_MSG(str)
#define DEBUG_MSGW(str)
#endif


class Device {

 public:
  explicit Device() {
    DEBUG_MSG("default construct");
  }

  explicit Device(std::string  str, uint64_t *array , size_t size) : s_m(std::move(str))
  {
    DEBUG_MSG("construct big");
    array_size_m = size;
    this->array_m = new uint64_t[array_size_m];
    std::memcpy(array_m ,array, array_size_m*sizeof(uint64_t));

  }

  ~Device() {
    delete[] this->array_m;
    DEBUG_MSG("destruct");
  }

  Device& operator=(const Device& other)
  {
    DEBUG_MSG("copy assigned");
    array_size_m = other.array_size_m;
    this->array_m = new uint64_t[array_size_m];
    memcpy(array_m ,other.array_m, array_size_m*sizeof(uint64_t));
    s_m = other.s_m;
    return *this;
  }

  Device& operator=(Device&& other) noexcept
  {
    DEBUG_MSG("move assigned");
    s_m = std::move(other.s_m);
    array_m = other.array_m;
    array_size_m = other.array_size_m;

    other.array_m = nullptr;
    other.array_size_m = 0;
    return *this;
  }


  Device& operator=(Device *other) noexcept
  {
    DEBUG_MSG("Pointer assigned");
    s_m = std::move(other->s_m);
    array_m = other->array_m;
    array_size_m = other->array_size_m;

    other->array_m = nullptr;
    other->array_size_m = 0;
    return *this;
  }

  void PrintStr() {
#ifdef DEBUG
    DEBUG_MSG("String: "+s_m);
    DEBUG_MSG("Array uint64 size: " + std::to_string(array_size_m));
    for(size_t n=0; n<array_size_m; n++) {
      DEBUG_MSGW(std::to_string(array_m[n])+" ");
    }
    DEBUG_MSG("");
#endif
  }


 private:
  std::string s_m;
  uint64_t *array_m;
  size_t array_size_m;

};









void work_with_device(Device& pre)
{
  DEBUG_MSG("[lvalue]")
  std::cout << "Device pre address = " << static_cast<void*>(&pre) << std::endl;
  Device Lval;
  Lval = pre;
  std::cout << "Device Lval address = " << static_cast<void*>(&Lval) << std::endl;
  Lval.PrintStr();
}

void work_with_device(Device&& pre)
{
  DEBUG_MSG("[rvalue]")
  std::cout << "Device pre address = " << static_cast<void*>(&pre) << std::endl;
  Device Rval;
  Rval = std::move(pre);
  std::cout << "Device Rval address = " << static_cast<void*>(&Rval) << std::endl;
  Rval.PrintStr();
}

void work_with_device(Device *pre)
{
  DEBUG_MSG("[pointer]")
  std::cout << "Device pre address = " << static_cast<void*>(pre) << std::endl;
  Device Ptr;
  Ptr = *pre;
  std::cout << "Device Ptr address = " << static_cast<void*>(&Ptr) << std::endl;
  Ptr.PrintStr();
}

int main()
{

  std::cout <<"\n\n";

  //data array
  std::array<uint64_t,31> arr {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30};
  uint32_t retry = 1;
  //---------------------------------------------------------------------
//Copy
  {
    std::chrono::steady_clock::time_point fooCopyBegin =
        std::chrono::steady_clock::now();

    for (uint32_t i = 0; i < retry; i++) {
      Device dev_1("Copy", arr.data(), arr.size());
      work_with_device(dev_1);
    }

    std::chrono::steady_clock::time_point fooCopyEnd =
        std::chrono::steady_clock::now();

    std::cout << "Copy assignment time: "
              << std::chrono::duration_cast<std::chrono::microseconds>(
                  fooCopyEnd - fooCopyBegin)
                  .count()
              << "[µs]" << std::endl;
  }
  std::cout <<"\n\n";
//---------------------------------------------------------------------
  {
//Pointer
    std::chrono::steady_clock::time_point fooPointerBegin =
        std::chrono::steady_clock::now();

    for (uint32_t i = 0; i < retry; i++) {
      //for heap
//      std::unique_ptr<Device> dev_1 = std::make_unique<Device>("Pointer", arr.data(), arr.size());
//      work_with_device(dev_1.to_get());

      //for stack
      Device dev_1 = Device("Pointer",arr.data(),arr.size());
      work_with_device(&dev_1);
    }

    std::chrono::steady_clock::time_point fooPointerEnd =
        std::chrono::steady_clock::now();

    std::cout << "Pointer assignment time: "
              << std::chrono::duration_cast<std::chrono::microseconds>(
                  fooPointerEnd - fooPointerBegin)
                  .count()
              << "[µs]" << std::endl;
  }
  std::cout <<"\n\n";


  //---------------------------------------------------------------------
  {
    // Move semantic
    std::chrono::steady_clock::time_point fooMoveBegin =
        std::chrono::steady_clock::now();

    for (uint32_t i = 0; i < retry; i++) {
      work_with_device(Device("Move", arr.data(), arr.size()));
    }

    std::chrono::steady_clock::time_point fooMoveEnd =
        std::chrono::steady_clock::now();
    std::cout << "Move assignment time: "
              << std::chrono::duration_cast<std::chrono::microseconds>(
                  fooMoveEnd - fooMoveBegin)
                  .count()
              << "[µs]" << std::endl;
  }
  //---------------------------------------------------------------------
//  std::cin.get();


}

