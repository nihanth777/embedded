namespace Dispenser_new
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            label1 = new Label();
            label2 = new Label();
            label3 = new Label();
            label4 = new Label();
            label5 = new Label();
            label6 = new Label();
            connect_btn = new Button();
            SuspendLayout();
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Font = new Font("Microsoft Sans Serif", 36F, FontStyle.Regular, GraphicsUnit.Point);
            label1.Location = new Point(163, 134);
            label1.Name = "label1";
            label1.Size = new Size(93, 55);
            label1.TabIndex = 0;
            label1.Text = "Vol";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Font = new Font("Microsoft Sans Serif", 36F, FontStyle.Regular, GraphicsUnit.Point);
            label2.Location = new Point(384, 134);
            label2.Name = "label2";
            label2.Size = new Size(117, 55);
            label2.TabIndex = 1;
            label2.Text = "Liter";
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Font = new Font("Microsoft Sans Serif", 36F, FontStyle.Regular, GraphicsUnit.Point);
            label3.Location = new Point(252, 134);
            label3.Name = "label3";
            label3.Size = new Size(145, 55);
            label3.TabIndex = 2;
            label3.Text = "00.00";
            // 
            // label4
            // 
            label4.Font = new Font("Microsoft Sans Serif", 36F, FontStyle.Regular, GraphicsUnit.Point);
            label4.Location = new Point(283, 189);
            label4.Name = "label4";
            label4.Size = new Size(148, 57);
            label4.TabIndex = 3;
            label4.Text = "00.00";
            // 
            // label5
            // 
            label5.AutoSize = true;
            label5.Font = new Font("Microsoft Sans Serif", 36F, FontStyle.Regular, GraphicsUnit.Point);
            label5.Location = new Point(239, 189);
            label5.Name = "label5";
            label5.Size = new Size(51, 55);
            label5.TabIndex = 4;
            label5.Text = "£";
            // 
            // label6
            // 
            label6.Font = new Font("Microsoft Sans Serif", 12F, FontStyle.Regular, GraphicsUnit.Point);
            label6.Location = new Point(252, 267);
            label6.Name = "label6";
            label6.Size = new Size(160, 29);
            label6.TabIndex = 5;
            label6.Text = "Petrol = £1.45/Liter";
            // 
            // connect_btn
            // 
            connect_btn.Font = new Font("Microsoft Sans Serif", 9F, FontStyle.Regular, GraphicsUnit.Point);
            connect_btn.Location = new Point(280, 299);
            connect_btn.Name = "connect_btn";
            connect_btn.Size = new Size(75, 23);
            connect_btn.TabIndex = 6;
            connect_btn.Text = "Connect";
            connect_btn.UseVisualStyleBackColor = true;
            connect_btn.Click += connect_btn_Click;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(800, 450);
            Controls.Add(connect_btn);
            Controls.Add(label6);
            Controls.Add(label5);
            Controls.Add(label4);
            Controls.Add(label3);
            Controls.Add(label2);
            Controls.Add(label1);
            KeyPreview = true;
            Name = "Form1";
            Text = "Form1";
            KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Form1_KeyPress);
            KeyUp += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyUp);
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Label label1;
        private Label label2;
        private Label label3;
        private Label label4;
        private Label label5;
        private Label label6;
        private Button connect_btn;
    }
}