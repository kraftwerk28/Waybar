#pragma once

#include <unordered_map>
#include <fmt/format.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/icontheme.h>
#include <gtkmm/image.h>
#include <unordered_map>
#include "AModule.hpp"
#include "bar.hpp"
#include "client.hpp"
#include "modules/sway/ipc/client.hpp"
#include "util/json.hpp"

namespace waybar::modules::sway {

class Workspaces : public AModule, public sigc::trackable {
 public:
  Workspaces(const std::string&, const waybar::Bar&, const Json::Value&);
  ~Workspaces() = default;
  auto update() -> void;

 private:
  static inline const std::string workspace_switch_cmd_ = "workspace {} \"{}\"";
  struct btn_ui {
    Gtk::Box btn_box;
    Gtk::Button btn_label;
    std::vector<Gtk::Button> app_icons;
    int focused_app = -1;
  };

  static int convertWorkspaceNameToNum(std::string name);

  void              onCmd(const struct Ipc::ipc_response&);
  void              onEvent(const struct Ipc::ipc_response&);
  bool              filterButtons();
  Workspaces::btn_ui&      addButton(const Json::Value&);
  void              onButtonReady(const Json::Value&, Gtk::Button&);
  std::string       getIcon(const std::string&, const Json::Value&);
  const std::string getCycleWorkspace(std::vector<Json::Value>::iterator, bool prev) const;
  uint16_t          getWorkspaceIndex(const std::string& name) const;
  std::string       trimWorkspaceName(std::string);
  bool              handleScroll(GdkEventScroll*);
  std::vector<Gtk::IconTheme> getAppIcons(const std::string &workspace_name);

  const Bar&                                   bar_;
  std::vector<Json::Value>                     workspaces_;
  std::vector<std::string>                     workspaces_order_;
  Gtk::Box                                     box_;
  util::JsonParser                             parser_;
  // std::unordered_map<std::string, Gtk::Button> buttons_;
  std::unordered_map<std::string, Workspaces::btn_ui> buttons_;
  std::mutex                                   mutex_;
  Ipc                                          ipc_;
  int focused_con_;
};

}  // namespace waybar::modules::sway
