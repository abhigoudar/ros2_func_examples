#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/u_int8.hpp>
#include "rcl_interfaces/msg/set_parameters_result.hpp"

class TestDynamicReconfigure : public rclcpp::Node
{
    public:
    //
    TestDynamicReconfigure() :
     Node("test_dynamic_reconfigure", rclcpp::NodeOptions())
    {

    }
    //
    ~TestDynamicReconfigure()
    {

    }
    //
    void parametersCallback(const rclcpp::Parameter& p)
    {
        RCLCPP_INFO(this->get_logger(),
            "callback_linear_x: Received an update to parameter \"%s\" "
            "of type %s: \"%d\"",
            p.get_name().c_str(), p.get_type_name().c_str(),
            p.as_int());
    }
    //
    void setupParameters()
    {
        rcl_interfaces::msg::ParameterDescriptor descriptor;
        rcl_interfaces::msg::IntegerRange range;
        range.set__from_value(0).set__to_value(100).set__step(1);
        descriptor.integer_range = {range};

        this->declare_parameter("test_param", 1, descriptor);
        param_handler  = std::make_shared<rclcpp::ParameterEventHandler>(this);
        param_cb = param_handler->add_parameter_callback("test_param",
            std::bind(&TestDynamicReconfigure::parametersCallback, this, std::placeholders::_1));

    }
    private:
    //
    std::shared_ptr<rclcpp::ParameterEventHandler> param_handler;
    //
    rclcpp::ParameterCallbackHandle::SharedPtr param_cb;
};


int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    std::shared_ptr<TestDynamicReconfigure> node = std::make_shared<TestDynamicReconfigure>();
    node->setupParameters();
    rclcpp::spin(node->get_node_base_interface());
    rclcpp::shutdown();
    return 0;
}