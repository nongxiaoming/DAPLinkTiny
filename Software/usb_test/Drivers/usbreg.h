#ifndef __USBREG_H
#define __USBREG_H


/** @defgroup USB_REGS_Exported_Defines
  * @{
  */ 

#define USBFS_OTG_BASE_ADDR                 0x50000000        

#define USB_OTG_GLOBAL_R_OFFSET          0x000
#define USB_OTG_DEV_R_OFFSET              0x800
#define USB_OTG_DEV_INEP_R_OFFSET         0x900
#define USB_OTG_EPT_R_OFFSET              0x20
#define USB_OTG_DEV_OUTEP_R_OFFSET        0xB00
#define USB_OTG_HOST_GLOBAL_R_OFFSET      0x400
#define USB_OTG_HOST_PORT_R_OFFSET        0x440
#define USB_OTG_HOST_CH_R_OFFSET           0x500
#define USB_OTG_CH_R_OFFSET                0x20
#define USB_OTG_PCGCCTL_OFFSET               0xE00
#define USB_OTG_DATA_FIFO_OFFSET             0x1000
#define USB_OTG_DATA_FIFO_SIZE               0x1000


#define USB_OTG_MAX_TX_FIFO                 15

#define USB_OTG_HS_MAX_PACKET_SIZE           512
#define USBFS_OTG_MAX_PACKET_SIZE           64
#define USB_OTG_MAX_EPT0_SIZE                 64

/* added by Keil */
#define OTG_FS_BASE           (USBFS_OTG_BASE_ADDR)
#define OTG_FS_DFIFO0_BASE    (OTG_FS_BASE + 0x01000)
#define OTG_FS_DFIFO1_BASE    (OTG_FS_BASE + 0x02000)
#define OTG_FS_DFIFO2_BASE    (OTG_FS_BASE + 0x03000)
#define OTG_FS_DFIFO3_BASE    (OTG_FS_BASE + 0x04000)
#define OTG_FS_DFIFO4_BASE    (OTG_FS_BASE + 0x05000)
#define OTG_FS_DFIFO5_BASE    (OTG_FS_BASE + 0x06000)
#define OTG_FS_DFIFO6_BASE    (OTG_FS_BASE + 0x07000)
#define OTG_FS_DFIFO7_BASE    (OTG_FS_BASE + 0x08000)

/* added by Keil */
/**
  * @brief USB OTG FS
  */
#pragma anon_unions

typedef struct
{
  __IO uint32_t GOTGCTL;
  __IO uint32_t GOTGINT;
  __IO uint32_t GAHBCFG;
  __IO uint32_t GUSBCFG;
  __IO uint32_t GRSTCTL;
  __IO uint32_t GINTSTS;
  __IO uint32_t GINTMSK;
  __IO uint32_t GRXSTSR;
  __IO uint32_t GRXSTSP;
  __IO uint32_t GRXFSIZ;
  union {
    __IO uint32_t HNPTXFSIZ;
    __IO uint32_t DIEPTXF0;
  };
  __IO uint32_t HNPTXSTS;
  __IO uint32_t GI2CCTL;
       uint32_t RESERVED0;
  __IO uint32_t GCCFG;
  __IO uint32_t CID;
       uint32_t RESERVED1[48];
  __IO uint32_t HPTXFSIZ;
  __IO uint32_t DIEPTXF1;
  __IO uint32_t DIEPTXF2;
  __IO uint32_t DIEPTXF3;
  __IO uint32_t DIEPTXF4;
  __IO uint32_t DIEPTXF5;
  __IO uint32_t DIEPTXF6;
  __IO uint32_t DIEPTXF7;
  __IO uint32_t DIEPTXF8;
  __IO uint32_t DIEPTXF9;
  __IO uint32_t DIEPTXF10;
  __IO uint32_t DIEPTXF11;
  __IO uint32_t DIEPTXF12;
  __IO uint32_t DIEPTXF13;
  __IO uint32_t DIEPTXF14;
  __IO uint32_t DIEPTXF15;
       uint32_t RESERVED2[176];
  __IO uint32_t HCFG;
  __IO uint32_t HFIR;
  __IO uint32_t HFNUM;
       uint32_t RESERVED3;
  __IO uint32_t HPTXSTS;
  __IO uint32_t HAINT;
  __IO uint32_t HAINTMSK;
       uint32_t RESERVED4[9];
  __IO uint32_t HPRT;
       uint32_t RESERVED5[47];
  __IO uint32_t HCCHAR0;
       uint32_t RESERVED6;
  __IO uint32_t HCINT0;
  __IO uint32_t HCINTMSK0;
  __IO uint32_t HCTSIZ0;
       uint32_t RESERVED7[3];
  __IO uint32_t HCCHAR1;
       uint32_t RESERVED8;
  __IO uint32_t HCINT1;
  __IO uint32_t HCINTMSK1;
  __IO uint32_t HCTSIZ1;
       uint32_t RESERVED9[3];
  __IO uint32_t HCCHAR2;
       uint32_t RESERVED10;
  __IO uint32_t HCINT2;
  __IO uint32_t HCINTMSK2;
  __IO uint32_t HCTSIZ2;
       uint32_t RESERVED11[3];
  __IO uint32_t HCCHR3;
       uint32_t RESERVED12;
  __IO uint32_t HCINT3;
  __IO uint32_t HCINTMSK3;
  __IO uint32_t HCTSIZ3;
       uint32_t RESERVED13[3];
  __IO uint32_t HCCHR4;
       uint32_t RESERVED14;
  __IO uint32_t HCINT4;
  __IO uint32_t HCINTMSK4;
  __IO uint32_t HCTSIZ4;
       uint32_t RESERVED15[3];
  __IO uint32_t HCCHR5;
       uint32_t RESERVED16;
  __IO uint32_t HCINT5;
  __IO uint32_t HCINTMSK5;
  __IO uint32_t HCTSIZ5;
       uint32_t RESERVED17[3];
  __IO uint32_t HCCHR6;
       uint32_t RESERVED18;
  __IO uint32_t HCINT6;
  __IO uint32_t HCINTMSK6;
  __IO uint32_t HCTSIZ6;
       uint32_t RESERVED19[3];
  __IO uint32_t HCCHR7;
       uint32_t RESERVED20;
  __IO uint32_t HCINT7;
  __IO uint32_t HCINTMSK7;
  __IO uint32_t HCTSIZ7;
       uint32_t RESERVED21[3];
  __IO uint32_t HCCHR8;
       uint32_t RESERVED22;
  __IO uint32_t HCINT8;
  __IO uint32_t HCINTMSK8;
  __IO uint32_t HCTSIZ8;
       uint32_t RESERVED23[3];
  __IO uint32_t HCCHR9;
       uint32_t RESERVED24;
  __IO uint32_t HCINT9;
  __IO uint32_t HCINTMSK9;
  __IO uint32_t HCTSIZ9;
       uint32_t RESERVED25[3];
  __IO uint32_t HCCHR10;
       uint32_t RESERVED26;
  __IO uint32_t HCINT10;
  __IO uint32_t HCINTMSK10;
  __IO uint32_t HCTSIZ10;
       uint32_t RESERVED27[3];
  __IO uint32_t HCCHR11;
       uint32_t RESERVED28;
  __IO uint32_t HCINT11;
  __IO uint32_t HCINTMSK11;
  __IO uint32_t HCTSIZ11;
       uint32_t RESERVED29[3];
  __IO uint32_t HCCHR12;
       uint32_t RESERVED30;
  __IO uint32_t HCINT12;
  __IO uint32_t HCINTMSK12;
  __IO uint32_t HCTSIZ12;
       uint32_t RESERVED31[3];
  __IO uint32_t HCCHR13;
       uint32_t RESERVED32;
  __IO uint32_t HCINT13;
  __IO uint32_t HCINTMSK13;
  __IO uint32_t HCTSIZ13;
       uint32_t RESERVED33[3];
  __IO uint32_t HCCHR14;
       uint32_t RESERVED34;
  __IO uint32_t HCINT14;
  __IO uint32_t HCINTMSK14;
  __IO uint32_t HCTSIZ14;
       uint32_t RESERVED35[3];
  __IO uint32_t HCCHR15;
       uint32_t RESERVED36;
  __IO uint32_t HCINT15;
  __IO uint32_t HCINTMSK15;
  __IO uint32_t HCTSIZ15;
       uint32_t RESERVED37[3];
       uint32_t RESERVED38[64];
  __IO uint32_t DCFG;
  __IO uint32_t DCTL;
  __IO uint32_t DSTS;
       uint32_t RESERVED39;
  __IO uint32_t DIEPMSK;
  __IO uint32_t DOEPMSK;
  __IO uint32_t DAINT;
  __IO uint32_t DAINTMSK;
       uint32_t RESERVED40[2];
  __IO uint32_t DVBUSDIS;
  __IO uint32_t DVBUSPULSE;
       uint32_t RESERVED41;
  __IO uint32_t DIEPEMPMSK;
       uint32_t RESERVED42[50];
  __IO uint32_t DIEPCTL0;
       uint32_t RESERVED43;
  __IO uint32_t DIEPINT0;
       uint32_t RESERVED44;
  __IO uint32_t DIEPTSIZ0;
       uint32_t RESERVED45;
  __IO uint32_t DTXFSTS0;
       uint32_t RESERVED46;
  __IO uint32_t DIEPCTL1;
       uint32_t RESERVED47;
  __IO uint32_t DIEPINT1;
       uint32_t RESERVED48;
  __IO uint32_t DIEPTSIZ1;
       uint32_t RESERVED49;
  __IO uint32_t DTXFSTS1;
       uint32_t RESERVED50;
  __IO uint32_t DIEPCTL2;
       uint32_t RESERVED51;
  __IO uint32_t DIEPINT2;
       uint32_t RESERVED52;
  __IO uint32_t DIEPTSIZ2;
       uint32_t RESERVED53;
  __IO uint32_t DTXFSTS2;
       uint32_t RESERVED54;
  __IO uint32_t DIEPCTL3;
       uint32_t RESERVED55;
  __IO uint32_t DIEPINT3;
       uint32_t RESERVED56;
  __IO uint32_t DIEPTSIZ3;
       uint32_t RESERVED57;
  __IO uint32_t DTXFSTS3;
       uint32_t RESERVED58;
  __IO uint32_t DIEPCTL4;
       uint32_t RESERVED59;
  __IO uint32_t DIEPINT4;
       uint32_t RESERVED60;
  __IO uint32_t DIEPTSIZ4;
       uint32_t RESERVED61;
  __IO uint32_t DTXFSTS4;
       uint32_t RESERVED62;
  __IO uint32_t DIEPCTL5;
       uint32_t RESERVED63;
  __IO uint32_t DIEPINT5;
       uint32_t RESERVED64;
  __IO uint32_t DIEPTSIZ5;
       uint32_t RESERVED65[3];
  __IO uint32_t DIEPCTL6;
       uint32_t RESERVED66;
  __IO uint32_t DIEPINT6;
       uint32_t RESERVED67;
  __IO uint32_t DIEPTSIZ6;
       uint32_t RESERVED68[3];
  __IO uint32_t DIEPCTL7;
       uint32_t RESERVED69;
  __IO uint32_t DIEPINT7;
       uint32_t RESERVED70;
  __IO uint32_t DIEPTSIZ7;
       uint32_t RESERVED71[3];
  __IO uint32_t DIEPCTL8;
       uint32_t RESERVED72;
  __IO uint32_t DIEPINT8;
       uint32_t RESERVED73;
  __IO uint32_t DIEPTSIZ8;
       uint32_t RESERVED74[3];
  __IO uint32_t DIEPCTL9;
       uint32_t RESERVED75;
  __IO uint32_t DIEPINT9;
       uint32_t RESERVED76;
  __IO uint32_t DIEPTSIZ9;
       uint32_t RESERVED77[3];
  __IO uint32_t DIEPCTL10;
       uint32_t RESERVED78;
  __IO uint32_t DIEPINT10;
       uint32_t RESERVED79;
  __IO uint32_t DIEPTSIZ10;
       uint32_t RESERVED80[3];
  __IO uint32_t DIEPCTL11;
       uint32_t RESERVED81;
  __IO uint32_t DIEPINT11;
       uint32_t RESERVED82;
  __IO uint32_t DIEPTSIZ11;
       uint32_t RESERVED83[3];
  __IO uint32_t DIEPCTL12;
       uint32_t RESERVED84;
  __IO uint32_t DIEPINT12;
       uint32_t RESERVED85;
  __IO uint32_t DIEPTSIZ86;
       uint32_t RESERVED86[3];
  __IO uint32_t DIEPCTL13;
       uint32_t RESERVED87;
  __IO uint32_t DIEPINT13;
       uint32_t RESERVED88;
  __IO uint32_t DIEPTSIZ13;
       uint32_t RESERVED89[3];
  __IO uint32_t DIEPCTL14;
       uint32_t RESERVED90;
  __IO uint32_t DIEPINT14;
       uint32_t RESERVED91;
  __IO uint32_t DIEPTSIZ14;
       uint32_t RESERVED92[3];
  __IO uint32_t DIEPCTL15;
       uint32_t RESERVED93;
  __IO uint32_t DIEPINT15;
       uint32_t RESERVED94;
  __IO uint32_t DIEPTSIZ15;
       uint32_t RESERVED95[3];
  __IO uint32_t DOEPCTL0;
       uint32_t RESERVED96;
  __IO uint32_t DOEPINT0;
       uint32_t RESERVED97;
  __IO uint32_t DOEPTSIZ0;
       uint32_t RESERVED98[3];
  __IO uint32_t DOEPCTL1;
       uint32_t RESERVED99;
  __IO uint32_t DOEPINT1;
       uint32_t RESERVED100;
  __IO uint32_t DOEPTSIZ1;
       uint32_t RESERVED101[3];
  __IO uint32_t DOEPCTL2;
       uint32_t RESERVED102;
  __IO uint32_t DOEPINT2;
       uint32_t RESERVED103;
  __IO uint32_t DOEPTSIZ2;
       uint32_t RESERVED104[3];
  __IO uint32_t DOEPCTL3;
       uint32_t RESERVED105;
  __IO uint32_t DOEPINT3;
       uint32_t RESERVED106;
  __IO uint32_t DOEPTSIZ3;
       uint32_t RESERVED107[3];
  __IO uint32_t DOEPCTL4;
       uint32_t RESERVED108;
  __IO uint32_t DOEPINT4;
       uint32_t RESERVED109;
  __IO uint32_t DOEPTSIZ4;
       uint32_t RESERVED110[3];
  __IO uint32_t DOEPCTL5;
       uint32_t RESERVED111;
  __IO uint32_t DOEPINT5;
       uint32_t RESERVED112;
  __IO uint32_t DOEPTSIZ5;
       uint32_t RESERVED113[3];
  __IO uint32_t DOEPCTL6;
       uint32_t RESERVED114;
  __IO uint32_t DOEPINT6;
       uint32_t RESERVED115;
  __IO uint32_t DOEPTSIZ6;
       uint32_t RESERVED116[3];
  __IO uint32_t DOEPCTL7;
       uint32_t RESERVED117;
  __IO uint32_t DOEPINT7;
       uint32_t RESERVED118;
  __IO uint32_t DOEPTSIZ7;
       uint32_t RESERVED119[3];
  __IO uint32_t DOEPCTL8;
       uint32_t RESERVED120;
  __IO uint32_t DOEPINT8;
       uint32_t RESERVED121;
  __IO uint32_t DOEPTSIZ8;
       uint32_t RESERVED122[3];
  __IO uint32_t DOEPCTL9;
       uint32_t RESERVED123;
  __IO uint32_t DOEPINT9;
       uint32_t RESERVED124;
  __IO uint32_t DOEPTSIZ9;
       uint32_t RESERVED125[3];
  __IO uint32_t DOEPCTL10;
       uint32_t RESERVED126;
  __IO uint32_t DOEPINT10;
       uint32_t RESERVED127;
  __IO uint32_t DOEPTSIZ10;
       uint32_t RESERVED128[3];
  __IO uint32_t DOEPCTL11;
       uint32_t RESERVED129;
  __IO uint32_t DOEPINT11;
       uint32_t RESERVED130;
  __IO uint32_t DOEPTSIZ11;
       uint32_t RESERVED131[3];
  __IO uint32_t DOEPCTL12;
       uint32_t RESERVED132;
  __IO uint32_t DOEPINT12;
       uint32_t RESERVED133;
  __IO uint32_t DOEPTSIZ12;
       uint32_t RESERVED134[3];
  __IO uint32_t DOEPCTL13;
       uint32_t RESERVED135;
  __IO uint32_t DOEPINT13;
       uint32_t RESERVED136;
  __IO uint32_t DOEPTSIZ13;
       uint32_t RESERVED137[3];
  __IO uint32_t DOEPCTL14;
       uint32_t RESERVED138;
  __IO uint32_t DOEPINT14;
       uint32_t RESERVED139;
  __IO uint32_t DOEPTSIZ14;
       uint32_t RESERVED140[3];
  __IO uint32_t DOEPCTL15;
       uint32_t RESERVED141;
  __IO uint32_t DOEPINT15;
       uint32_t RESERVED142;
  __IO uint32_t DOEPTSIZ15;
       uint32_t RESERVED143[3];
       uint32_t RESERVED144[64];
  __IO uint32_t PCGCCTL;
} OTG_FS_TypeDef;

#define OTG_FS              ((OTG_FS_TypeDef *) OTG_FS_BASE)  
#define OTG_FS_DFIFO0       (((uint32_t *) OTG_FS_DFIFO0_BASE)  )
#define OTG_FS_DFIFO1       (((uint32_t *) OTG_FS_DFIFO1_BASE)  )
#define OTG_FS_DFIFO2       (((uint32_t *) OTG_FS_DFIFO2_BASE)  )
#define OTG_FS_DFIFO3       (((uint32_t *) OTG_FS_DFIFO3_BASE)  )
#define OTG_FS_DFIFO4       (((uint32_t *) OTG_FS_DFIFO4_BASE)  )
#define OTG_FS_DFIFO5       (((uint32_t *) OTG_FS_DFIFO5_BASE)  )
#define OTG_FS_DFIFO6       (((uint32_t *) OTG_FS_DFIFO6_BASE)  )
#define OTG_FS_DFIFO7       (((uint32_t *) OTG_FS_DFIFO7_BASE)  )



#define ISTR_RESUME        (0x01UL<<31)
#define ISTR_INCOMISOOUTM  (0x01UL<<21)
#define ISTR_INCOMISOIN    (0x01UL<<20)
#define ISTR_OEPTINT       (0x01UL<<19)
#define ISTR_IEPTINT       (0x01UL<<18)
#define ISTR_EOPF          (0x01UL<<15)
#define ISTR_ISOOUTDR      (0x01UL<<14)
#define ISTR_ENUMDONE      (0x01UL<<13)
#define ISTR_RESET         (0x01UL<<12)
#define ISTR_SUSP          (0x01UL<<11)
#define ISTR_ERLYSUSP      (0x01UL<<10)
#define ISTR_RXFLVL        (0x01UL<<4)
#define ISTR_SOF           (0x01UL<<3)

#endif  /* __USBREG_H */
