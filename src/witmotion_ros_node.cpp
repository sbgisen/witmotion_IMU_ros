#include "witmotion_ros.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  // Qt アプリケーションの初期化
  QCoreApplication app(argc, argv);

  // ROS 2 初期化オプション
  rclcpp::InitOptions options{};
  options.shutdown_on_signal = true;
  rclcpp::init(argc, argv, options);

  // シャットダウン時のコールバック
  rclcpp::on_shutdown([]() {
    RCLCPP_INFO(rclcpp::get_logger("MinimalPublisher"), "Shutting down QT...");
    QCoreApplication::exit(0);
    QThreadPool::globalInstance()->waitForDone();
    RCLCPP_INFO(rclcpp::get_logger("MinimalPublisher"), "Shutting down node...");
    if (!rclcpp::shutdown()) {
      RCLCPP_WARN(rclcpp::get_logger("MinimalPublisher"), "Shutting down node failed...");
    }
  });

  // ★ NodeOptions に ROS 2 の CLI 引数（namespace など）を反映させる
  rclcpp::NodeOptions node_options;
  node_options.allow_undeclared_parameters(true);
  node_options.automatically_declare_parameters_from_overrides(true);

  // ★ Sensor Controller のノードを起動
  auto &controller = ROSWitmotionSensorController::Instance();
  auto node = controller.Start(node_options);

  // ★ ノードを Executor に登録して別スレッドで spin
  rclcpp::executors::SingleThreadedExecutor executor;
  executor.add_node(node);

  std::thread spin_thread([&executor]() {
    executor.spin();
  });
  spin_thread.detach();

  RCLCPP_INFO(rclcpp::get_logger("MinimalPublisher"), "QT spin !!!!!");

  // Qt のイベントループ開始
  return app.exec();
}
