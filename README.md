# MessageQueue

Simple message and event queue for C++ built on top of a circular buffer.

## Linking

Just put `MessageQueue.h` in your include directories.

## Usage

```cpp
// Message type for the queue
struct MyMessage
{
	std::string Value;
};

// Create a queue with MyMessage type and buffer of size = 3 
auto queue = MessageQueue<MyMessage, 3>();

// Create a view to consume queue
auto view1 = queue.CreateView();

// Publish messages to the queue
queue.Publish({ "Message #1" });
queue.Publish({ "Message #2" });

// Read one by one
view1.Next(); // *MyMessage("Message #1")
view1.Next(); // *MyMessage("Message #2")
view1.Next(); // nullptr 

// Create another view to consume queue
auto view2 = queue.CreateView();

// Publish new messages to the queue
queue.Publish({ "Message #3" });
queue.Publish({ "Message #4" });

// View reads queue from the moment of creation and ignores previous messages
view2.Next(); // *MyMessage("Message #3")
view2.Next(); // *MyMessage("Message #4")
view2.Next(); // nullptr 

// You can iterate through all remaining messages using functional style
view1.ForEach([](const MyMessage* message) {
	// 1 iteration: message = *MyMessage("Message #3")
	// 2 iteration: message = *MyMessage("Message #4")
});
```