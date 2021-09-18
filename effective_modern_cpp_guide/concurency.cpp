
void be_aware_future_destructor()
{
    //jak działa future ? 

    // caller(future) <----- Shared State ( czyli mój przyszły wynik) <----- calee (std::promise)

    //jeśli future spełnia następujące warunki:
    //It refers to a shared state that was created due to a call to std::async.
    //The task’s launch policy is std::launch::async
    //The future is the last future referring to the shared state

    //to wtedy:
    // “Futures from std::async block in their destructors.”


}