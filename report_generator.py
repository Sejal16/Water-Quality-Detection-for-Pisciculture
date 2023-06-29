#!/usr/bin/env python
# coding: utf-8

# In[2]:


from fpdf import FPDF
from PIL import Image

def create_report():
    title = 'Monthly Report of Water Quality Measurement'
    class PDF(FPDF):

        def header(self):
            if(self.page_no() == 1):
                self.set_font('helvetica', 'B', 20)
                title_w = self.get_string_width(title) + 6
                doc_w = self.w
                self.set_x((doc_w - title_w) / 2)
                self.set_text_color(103,183,209)
                # Title
                self.cell(title_w, 10,title, border=False, ln=1, align='C')
                # Line break
                self.ln(10)

        def footer(self):
            # Set position of the footer
            self.set_y(-15)
            # set font
            self.set_font('helvetica', 'I', 8)
            # Page number
            self.cell(0, 10, f'Page {self.page_no()}/{{nb}}', align='C')

        def chapter_body(self, name):
            # read text file
            with open(name, encoding='utf-8') as fh:
                txt = fh.read()
            self.set_font('times', '', 12)
            self.set_text_color(0, 0, 0)
            self.multi_cell(0, 5, txt)
            self.ln()
        
        def sub_heading(self, text):
            self.set_font('helvetica', 'B', 14)
            self.set_text_color(103,183,209)
            self.cell(0, 8, text, ln=1)
            self.ln(4)


    pdf = PDF('P', 'mm', 'Letter')

    # get total page numbers
    pdf.alias_nb_pages()

    # Set auto page break
    pdf.set_auto_page_break(auto = True, margin = 15)

    pdf.add_page()

    img_w = 140 # Width of the image in points
    page_w = pdf.w # Width of the page in points
    margin = (page_w - img_w) / 2 # Left margin
    pdf.set_left_margin(margin)

    pdf.chapter_body('pdf_content/Introduction.txt')
    pdf.sub_heading('Chart 1: Temperature vs. Date')
    pdf.chapter_body('pdf_content/body1.txt')
    pdf.image('temperature_plot.png',x=None, y=None, w=img_w)
    pdf.ln(10)

    pdf.sub_heading('Chart 2: pH vs. Date')
    pdf.chapter_body('pdf_content/body2.txt')
    pdf.image('pH_plot.png',x=None, y=None, w=img_w)
    pdf.ln(10)

    pdf.sub_heading('Chart 3: Turbidity vs. Date')
    pdf.chapter_body('pdf_content/body3.txt')
    pdf.image('turbidity_plot.png', x=None, y=None, w=img_w)
    pdf.ln(10)

    pdf.sub_heading('Chart 4: Water Quality Status')
    pdf.chapter_body('pdf_content/body4.txt')
    pdf.image('water_condition_pie_chart.png', x=None, y=None, w=img_w)
    pdf.ln(10)
    pdf.chapter_body('pdf_content/conclusion.txt')
    pdf.output('report.pdf')


# In[ ]:




