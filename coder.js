// $Id$

if (Drupal.jsEnabled) {
  jQuery.fn.extend({
    check : function() { return this.each(function() { this.checked = true; }); },
    uncheck : function() { return this.each(function() { this.checked = false; }); }
  });

  $(document).ready(
    function() {
      $("input:checkbox").click(
        function() {
          core = this.form.elements.namedItem("edit-coder-core-modules");
          active = this.form.elements.namedItem("edit-coder-active-modules");
          if (this == core || this == active) {
            what = "input[@id^=edit-coder-modules]";
            if (core.checked || active.checked) {
              $("input[@id^=edit-coder-modules]").uncheck();
              if (core.checked) what += '.coder-core';
              if (active.checked) what += '.coder-active';
              $(what).check();
            }
            else {
              if (this == active) what += ".coder-active";
              else what += ".coder-core";
              $(what).uncheck();
            }
          }
          else if (this.id.substr(0, 18) == "edit-coder-modules") {
            core.checked = false;
            active.checked = false;
          }
          return true;
        }
      );
    }
  );
}
