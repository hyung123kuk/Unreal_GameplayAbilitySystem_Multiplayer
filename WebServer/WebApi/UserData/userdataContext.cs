using System;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Metadata;

namespace WebApi.Models
{
    public partial class userdataContext : DbContext
    {
        public userdataContext()
        {
        }

        public userdataContext(DbContextOptions<userdataContext> options)
            : base(options)
        {
        }

        public virtual DbSet<ChattingLog> ChattingLog { get; set; }
        public virtual DbSet<Member> Member { get; set; }
        public virtual DbSet<MemberItem> MemberItem { get; set; }
        public virtual DbSet<ServerList> ServerList { get; set; }
        public virtual DbSet<StoreItem> StoreItem { get; set; }
        public virtual DbSet<StorePurchaseLog> StorePurchaseLog { get; set; }

        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
            if (!optionsBuilder.IsConfigured)
            {
#warning To protect potentially sensitive information in your connection string, you should move it out of source code. See http://go.microsoft.com/fwlink/?LinkId=723263 for guidance on storing connection strings.
                optionsBuilder.UseMySql("server=34.64.175.166;port=3306;database=userdata;uid=root;pwd=gudrnr9630!", x => x.ServerVersion("5.6.51-mysql"));
            }
        }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            modelBuilder.Entity<ChattingLog>(entity =>
            {
                entity.ToTable("chatting_log");

                entity.HasIndex(e => e.UserId)
                    .HasName("FK_chattinglog_userid");

                entity.Property(e => e.Id)
                    .HasColumnName("id")
                    .HasColumnType("int(1) unsigned zerofill");

                entity.Property(e => e.ChatMessage)
                    .IsRequired()
                    .HasColumnName("chat_message")
                    .HasColumnType("char(100)")
                    .HasCharSet("utf8")
                    .HasCollation("utf8_general_ci");

                entity.Property(e => e.Place)
                    .IsRequired()
                    .HasColumnName("place")
                    .HasColumnType("char(12)")
                    .HasCharSet("utf8")
                    .HasCollation("utf8_general_ci");

                entity.Property(e => e.Time)
                    .HasColumnName("time")
                    .HasColumnType("datetime");

                entity.Property(e => e.UserId)
                    .IsRequired()
                    .HasColumnName("user_id")
                    .HasColumnType("char(12)")
                    .HasCharSet("utf8")
                    .HasCollation("utf8_general_ci");

                entity.HasOne(d => d.User)
                    .WithMany(p => p.ChattingLog)
                    .HasForeignKey(d => d.UserId)
                    .OnDelete(DeleteBehavior.ClientSetNull)
                    .HasConstraintName("FK_chattinglog_userid");
            });

            modelBuilder.Entity<Member>(entity =>
            {
                entity.ToTable("member");

                entity.Property(e => e.Id)
                    .HasColumnName("id")
                    .HasColumnType("char(12)")
                    .HasCharSet("utf8")
                    .HasCollation("utf8_general_ci");

                entity.Property(e => e.CreateTime)
                    .HasColumnName("createTime")
                    .HasColumnType("datetime");

                entity.Property(e => e.Exp)
                    .HasColumnName("exp")
                    .HasColumnType("int(1) unsigned");

                entity.Property(e => e.Gold)
                    .HasColumnName("gold")
                    .HasColumnType("int(1) unsigned");

                entity.Property(e => e.Introduction)
                    .IsRequired()
                    .HasColumnName("introduction")
                    .HasColumnType("char(35)")
                    .HasCharSet("utf8")
                    .HasCollation("utf8_general_ci");

                entity.Property(e => e.Password)
                    .IsRequired()
                    .HasColumnName("password")
                    .HasColumnType("varchar(256)")
                    .HasDefaultValueSql("'0'")
                    .HasCharSet("utf8")
                    .HasCollation("utf8_general_ci");
            });

            modelBuilder.Entity<MemberItem>(entity =>
            {
                entity.ToTable("member_item");

                entity.HasIndex(e => e.ItemId)
                    .HasName("FK_memberitem_storeitem");

                entity.HasIndex(e => e.UserId)
                    .HasName("FK_memberitem_member");

                entity.Property(e => e.Id)
                    .HasColumnName("id")
                    .HasColumnType("int(10) unsigned zerofill");

                entity.Property(e => e.ItemCount)
                    .HasColumnName("item_count")
                    .HasColumnType("int(1) unsigned")
                    .HasDefaultValueSql("'1'");

                entity.Property(e => e.ItemId)
                    .HasColumnName("item_id")
                    .HasColumnType("tinyint(1) unsigned zerofill");

                entity.Property(e => e.UserId)
                    .IsRequired()
                    .HasColumnName("user_id")
                    .HasColumnType("char(12)")
                    .HasCharSet("utf8")
                    .HasCollation("utf8_general_ci");

                entity.HasOne(d => d.Item)
                    .WithMany(p => p.MemberItem)
                    .HasForeignKey(d => d.ItemId)
                    .OnDelete(DeleteBehavior.ClientSetNull)
                    .HasConstraintName("FK_memberitem_storeitem");

                entity.HasOne(d => d.User)
                    .WithMany(p => p.MemberItem)
                    .HasForeignKey(d => d.UserId)
                    .OnDelete(DeleteBehavior.ClientSetNull)
                    .HasConstraintName("FK_memberitem_member");
            });

            modelBuilder.Entity<ServerList>(entity =>
            {
                entity.HasKey(e => e.Ip)
                    .HasName("PRIMARY");

                entity.ToTable("server_list");

                entity.Property(e => e.Ip)
                    .HasColumnName("ip")
                    .HasColumnType("char(25)")
                    .HasCharSet("utf8")
                    .HasCollation("utf8_general_ci");

                entity.Property(e => e.CrowdedLevel)
                    .HasColumnName("crowded_level")
                    .HasColumnType("int(1) unsigned");

                entity.Property(e => e.IsOperation)
                    .HasColumnName("is_operation")
                    .HasColumnType("bit(1)");

                entity.Property(e => e.Name)
                    .HasColumnName("name")
                    .HasColumnType("char(20)")
                    .HasCharSet("utf8")
                    .HasCollation("utf8_general_ci");
            });

            modelBuilder.Entity<StoreItem>(entity =>
            {
                entity.ToTable("store_item");

                entity.Property(e => e.Id)
                    .HasColumnName("id")
                    .HasColumnType("tinyint(1) unsigned zerofill");

                entity.Property(e => e.Cost)
                    .HasColumnName("cost")
                    .HasColumnType("int(1)");

                entity.Property(e => e.Explanation)
                    .IsRequired()
                    .HasColumnName("explanation")
                    .HasColumnType("char(50)")
                    .HasCharSet("utf8")
                    .HasCollation("utf8_general_ci");

                entity.Property(e => e.Image)
                    .IsRequired()
                    .HasColumnName("image")
                    .HasColumnType("char(20)")
                    .HasCharSet("utf8")
                    .HasCollation("utf8_general_ci");

                entity.Property(e => e.IsSale)
                    .HasColumnName("is_sale")
                    .HasColumnType("bit(1)");

                entity.Property(e => e.Name)
                    .IsRequired()
                    .HasColumnName("name")
                    .HasColumnType("char(20)")
                    .HasCharSet("utf8")
                    .HasCollation("utf8_general_ci");
            });

            modelBuilder.Entity<StorePurchaseLog>(entity =>
            {
                entity.ToTable("store_purchase_log");

                entity.HasIndex(e => e.PurchaseItem)
                    .HasName("purchase_item");

                entity.HasIndex(e => e.UserId)
                    .HasName("user_id");

                entity.Property(e => e.Id)
                    .HasColumnName("id")
                    .HasColumnType("int(10) unsigned zerofill");

                entity.Property(e => e.PurchaseItem)
                    .HasColumnName("purchase_item")
                    .HasColumnType("tinyint(1) unsigned zerofill");

                entity.Property(e => e.PurchaseTime)
                    .HasColumnName("purchase_time")
                    .HasColumnType("datetime");

                entity.Property(e => e.UserId)
                    .IsRequired()
                    .HasColumnName("user_id")
                    .HasColumnType("char(12)")
                    .HasCharSet("utf8")
                    .HasCollation("utf8_general_ci");

                entity.HasOne(d => d.PurchaseItemNavigation)
                    .WithMany(p => p.StorePurchaseLog)
                    .HasForeignKey(d => d.PurchaseItem)
                    .OnDelete(DeleteBehavior.ClientSetNull)
                    .HasConstraintName("FK_purchaselog_storeitem");

                entity.HasOne(d => d.User)
                    .WithMany(p => p.StorePurchaseLog)
                    .HasForeignKey(d => d.UserId)
                    .OnDelete(DeleteBehavior.ClientSetNull)
                    .HasConstraintName("FK_purchaselog_member");
            });

            OnModelCreatingPartial(modelBuilder);
        }

        partial void OnModelCreatingPartial(ModelBuilder modelBuilder);
    }
}
