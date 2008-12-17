#include <glade/glade.h>
#include <gtk/gtk.h>
#include <serial_interface/connection.h>
#include <serial_interface/errno.h>
#include <serial_interface/control_table.h>
#include <assert.h>

const double ANGLE_CONVERSION_FACTOR = 360.0/0x3FF;
struct ftdi_context ftdic;
unsigned short speed = 2;

void quit(GtkRange *range, gpointer user_data)
{
  ar_io_close(&ftdic);
  gtk_main_quit();
}

void on_hscale_head_value_changed(GtkRange *range, gpointer user_data) 
{
  struct ar_io_instruction_packet ip;
  struct ar_io_status_packet sp;
  int rc;
  double val = gtk_range_get_value(range);

  unsigned char params[] = 
    { 
      0x1E,
      (((unsigned char) val) & 0xFF00) >> 8,
      (((unsigned char) val) & 0x00FF),
      0x00,
      0x02
    };

  ip.id = 16;
  ip.instruction = 0x03;
  ip.params = params;
  ip.param_count = 5;

  rc = ar_io_write_instruction_packet(&ftdic, &ip);
  if (rc == -1)
    {
      fprintf(stderr, "Error writing instruction packet: %s\n",
	      ar_io_errstring(ar_io_errno));
      return;
    }

  sp.params = NULL;
  rc = ar_io_read_status_packet(&ftdic, &sp);
  if (rc == -1)
    {
      fprintf(stderr, "Error reading status packet: %s\n",
	      ar_io_errstring(ar_io_errno));
      return;
    }

  fprintf(stderr, "Data sent: %f, RC: %i\n", val, sp.error);
}
void on_hscale_torso_xy_value_changed(GtkRange *range, gpointer user_data) {}
void on_hscale_torso_xz_value_changed(GtkRange *range, gpointer user_data) {}
void on_hscale_left_shoulder_xz_value_changed(GtkRange *range, gpointer user_data) {}
void on_hscale_right_shoulder_xz_value_changed(GtkRange *range, gpointer user_data) {}
void on_hscale_left_shoulder_xy_value_changed(GtkRange *range, gpointer user_data) {}
void on_hscale_right_shoulder_xy_value_changed(GtkRange *range, gpointer user_data) {}
void on_hscale_left_shoulder_yz_value_changed(GtkRange *range, gpointer user_data) {}
void on_hscale_right_shoulder_yz_value_changed(GtkRange *range, gpointer user_data) {}
void on_hscale_left_elbow_xz_value_changed(GtkRange *range, gpointer user_data) {}
void on_hscale_right_elbow_xz_value_changed(GtkRange *range, gpointer user_data) {}
void on_hscale_left_hip_yz_value_changed(GtkRange *range, gpointer user_data) {}
void on_hscale_right_hip_yz_value_changed(GtkRange *range, gpointer user_data) {}
void on_hscale_left_hip_xy_value_changed(GtkRange *range, gpointer user_data) {}
void on_hscale_right_hip_xy_value_changed(GtkRange *range, gpointer user_data) {}
void on_hscale_left_hip_xz_value_changed(GtkRange *range, gpointer user_data) {}
void on_hscale_right_hip_xz_value_changed(GtkRange *range, gpointer user_data) {}
void on_hscale_left_knee_xz_value_changed(GtkRange *range, gpointer user_data) {}
void on_hscale_right_knee_xz_value_changed(GtkRange *range, gpointer user_data) {}
void on_hscale_left_foot_xz_value_changed(GtkRange *range, gpointer user_data) {}
void on_hscale_right_foot_xz_value_changed(GtkRange *range, gpointer user_data) {}
void on_hscale_left_foot_yz_value_changed(GtkRange *range, gpointer user_data) {}
void on_hscale_right_foot_yz_value_changed(GtkRange *range, gpointer user_data) {}

unsigned char angle_to_goal_pos(double angle)
{
  while(angle > 360.0)
    angle -= 360.0;
  while(angle < 0)
    angle += 360.0;
  return angle / ANGLE_CONVERSION_FACTOR;
}

double goal_pos_to_angle(unsigned char goal_pos)
{
  return ((double) goal_pos) * ANGLE_CONVERSION_FACTOR;
}

int main(int argc, char **argv)
{
  GladeXML *xml;
  GtkWidget *wnd_main, *hscale_head, *hscale_torso_xy, *hscale_torso_xz,
    *hscale_left_shoulder_xz, *hscale_right_shoulder_xz,
    *hscale_left_shoulder_xy, *hscale_right_shoulder_xy,
    *hscale_left_shoulder_yz, *hscale_right_shoulder_yz,
    *hscale_left_elbow_xz, *hscale_right_elbow_xz,
    *hscale_left_hip_yz, *hscale_right_hip_yz,
    *hscale_left_hip_xy, *hscale_right_hip_xy,
    *hscale_left_hip_xz, *hscale_right_hip_xz,
    *hscale_left_knee_xz, *hscale_right_knee_xz,
    *hscale_left_foot_xz, *hscale_right_foot_xz,
    *hscale_left_foot_yz, *hscale_right_foot_yz;
  gtk_init(&argc, &argv);

  xml = glade_xml_new("share/arnie-gui/gui.glade",
		      NULL,
		      NULL);

  wnd_main = glade_xml_get_widget(xml, "wnd_main");

  hscale_head = glade_xml_get_widget(xml, "hscale_head");
  hscale_torso_xy = glade_xml_get_widget(xml, "hscale_torso_xy");
  hscale_torso_xz = glade_xml_get_widget(xml, "hscale_torso_xz");
  hscale_left_shoulder_xz = glade_xml_get_widget(xml, 
						 "hscale_left_shoulder_xz");
  hscale_right_shoulder_xz = glade_xml_get_widget(xml, 
						  "hscale_right_shoulder_xz");
  hscale_left_shoulder_xy = glade_xml_get_widget(xml, 
						 "hscale_left_shoulder_xy");
  hscale_right_shoulder_xy = glade_xml_get_widget(xml, 
						  "hscale_right_shoulder_xy");
  hscale_left_shoulder_yz = glade_xml_get_widget(xml, 
						 "hscale_left_shoulder_yz");
  hscale_right_shoulder_yz = glade_xml_get_widget(xml, 
						  "hscale_right_shoulder_yz");
  hscale_left_elbow_xz = glade_xml_get_widget(xml, "hscale_left_elbow_xz");
  hscale_right_elbow_xz = glade_xml_get_widget(xml, "hscale_right_elbow_xz");
  hscale_left_hip_yz = glade_xml_get_widget(xml, "hscale_left_hip_yz");
  hscale_right_hip_yz = glade_xml_get_widget(xml, "hscale_right_hip_yz");
  hscale_left_hip_xy = glade_xml_get_widget(xml, "hscale_left_hip_xy");
  hscale_right_hip_xy = glade_xml_get_widget(xml, "hscale_right_hip_xy");
  hscale_left_hip_xz = glade_xml_get_widget(xml, "hscale_left_hip_xz");
  hscale_right_hip_xz = glade_xml_get_widget(xml, "hscale_right_hip_xz");
  hscale_left_knee_xz = glade_xml_get_widget(xml, "hscale_left_knee_xz");
  hscale_right_knee_xz = glade_xml_get_widget(xml, "hscale_right_knee_xz");
  hscale_left_foot_xz = glade_xml_get_widget(xml, "hscale_left_foot_xz");
  hscale_right_foot_xz = glade_xml_get_widget(xml, "hscale_right_foot_xz");
  hscale_left_foot_yz = glade_xml_get_widget(xml, "hscale_left_foot_yz");
  hscale_right_foot_yz = glade_xml_get_widget(xml, "hscale_right_foot_yz");

  unsigned char ids[] =
    { 
      16,17,18,32,33,34,35,48,49,50,51,64,65,66,67,68,69,80,81,82,83,84,85
    };
  GtkWidget *hscales[] =
    {
      hscale_head,hscale_torso_xy,hscale_torso_xz,hscale_left_shoulder_xz,hscale_right_shoulder_xz,hscale_left_shoulder_xy,hscale_right_shoulder_xy,hscale_left_shoulder_yz,hscale_right_shoulder_yz,hscale_left_elbow_xz,hscale_right_elbow_xz,hscale_left_hip_yz,hscale_right_hip_yz,hscale_left_hip_xy,hscale_right_hip_xy,hscale_left_hip_xz,hscale_right_hip_xz,hscale_left_knee_xz,hscale_right_knee_xz,hscale_left_foot_xz,hscale_right_foot_xz,hscale_left_foot_yz,hscale_right_foot_yz
    };

  gtk_widget_show(wnd_main);

  g_signal_connect(G_OBJECT(wnd_main), "delete-event",
		   G_CALLBACK(quit), NULL);

  int ar_rc;
  ar_rc = ar_io_open(&ftdic);
  assert(ar_rc != -1);

  g_signal_connect(G_OBJECT(hscale_head), "value-changed", G_CALLBACK(on_hscale_head_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_torso_xy), "value-changed", G_CALLBACK(on_hscale_torso_xy_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_torso_xz), "value-changed", G_CALLBACK(on_hscale_torso_xz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_left_shoulder_xz), "value-changed", G_CALLBACK(on_hscale_left_shoulder_xz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_right_shoulder_xz), "value-changed", G_CALLBACK(on_hscale_right_shoulder_xz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_left_shoulder_xy), "value-changed", G_CALLBACK(on_hscale_left_shoulder_xy_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_right_shoulder_xy), "value-changed", G_CALLBACK(on_hscale_right_shoulder_xy_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_left_shoulder_yz), "value-changed", G_CALLBACK(on_hscale_left_shoulder_yz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_right_shoulder_yz), "value-changed", G_CALLBACK(on_hscale_right_shoulder_yz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_left_elbow_xz), "value-changed", G_CALLBACK(on_hscale_left_elbow_xz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_right_elbow_xz), "value-changed", G_CALLBACK(on_hscale_right_elbow_xz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_left_hip_yz), "value-changed", G_CALLBACK(on_hscale_left_hip_yz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_right_hip_yz), "value-changed", G_CALLBACK(on_hscale_right_hip_yz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_left_hip_xy), "value-changed", G_CALLBACK(on_hscale_left_hip_xy_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_right_hip_xy), "value-changed", G_CALLBACK(on_hscale_right_hip_xy_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_left_hip_xz), "value-changed", G_CALLBACK(on_hscale_left_hip_xz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_right_hip_xz), "value-changed", G_CALLBACK(on_hscale_right_hip_xz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_left_knee_xz), "value-changed", G_CALLBACK(on_hscale_left_knee_xz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_right_knee_xz), "value-changed", G_CALLBACK(on_hscale_right_knee_xz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_left_foot_xz), "value-changed", G_CALLBACK(on_hscale_left_foot_xz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_right_foot_xz), "value-changed", G_CALLBACK(on_hscale_right_foot_xz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_left_foot_yz), "value-changed", G_CALLBACK(on_hscale_left_foot_yz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_right_foot_yz), "value-changed", G_CALLBACK(on_hscale_right_foot_yz_value_changed), NULL);


  gtk_main();
  return 0;
}
