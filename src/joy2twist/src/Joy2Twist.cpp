#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>
class Joy2Twist
{
    public:
    Joy2Twist();
    void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);
    void Publish();
    ros::NodeHandle nh_;
    int linear_, angular_;
    double l_scale_, a_scale_;
    ros::Publisher vel_pub_;
    ros::Subscriber joy_sub_;
    geometry_msgs::Twist vel;
};


Joy2Twist::Joy2Twist():
linear_(1),
angular_(2),
l_scale_(2.0),
a_scale_(2.0)
{
    nh_.param("axis_linear", linear_, linear_);
    nh_.param("axis_angular", angular_, angular_);
    nh_.param("scale_angular", a_scale_, a_scale_);
    nh_.param("scale_linear", l_scale_, l_scale_);
    vel_pub_ = nh_.advertise<geometry_msgs::Twist>("cmd_vel", 1);
    joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("joy", 10, &Joy2Twist::joyCallback, this);
}
void Joy2Twist::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
    
    vel.angular.z = a_scale_* joy->axes[angular_];
    vel.linear.x = l_scale_*joy->axes[linear_];
    //vel_pub_.publish(vel);
    
}
void Joy2Twist::Publish()
{
    vel_pub_.publish(vel);
}

int main(int argc, char** argv)
{
    
    ros::init(argc, argv, "joy2twist", ros::init_options::AnonymousName);
    Joy2Twist joy2twist;
    ros::Rate loop_rate(10);

    while(ros::ok())
    {
       joy2twist.Publish();
       ros::spinOnce();
       loop_rate.sleep();

    }
    
    
    //ros::spin();
}
