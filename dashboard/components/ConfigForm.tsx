"use client";

import { zodResolver } from "@hookform/resolvers/zod";
import { useForm } from "react-hook-form";
import { z } from "zod";
import { Button } from "@/components/ui/button";
import {
  Form,
  FormControl,
  FormField,
  FormItem,
  FormMessage,
} from "@/components/ui/form";
import { Switch } from "@/components/ui/switch";
import { Card } from "@/components/ui/card";
import { useCallback, useEffect, useState } from "react";
import { toast } from "sonner";

const formSchema = z.object({
  muted: z.boolean(),
  active: z.boolean(),
  network_status: z.boolean().optional()
});

type ConfigFormValues = z.infer<typeof formSchema>;

export function ConfigForm() {
  const [networkStatus, setNetworkStatus] = useState(false);
  const form = useForm<ConfigFormValues>({
    resolver: zodResolver(formSchema),
    defaultValues: {
      muted: false,
      active: false,
    },
  });


  const getDataFromThingSpeak = useCallback(async () => {
    try {
      const response = await fetch("/api/thingspeak", { method: "GET" });
      const data = await response.json();
      form.setValue('muted', data.data.field3 == 1);
      form.setValue('active', data.data.field4 == 1);
      setNetworkStatus(data.data.field5 == 1);
    } catch (error) {
      console.error("Error:", error);
    }
  }, [form]);

  const sendDataToThingSpeak = useCallback(async (field3: number, field4: number) => {
    try {
      const response = await fetch("/api/thingspeak", {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({
          field3,
          field4,
        }),
      });

      const data = await response.json();
      toast(data.message)
    } catch (error) {
      toast(error)
    }
  }, []);


  useEffect(() => {
    getDataFromThingSpeak();
  }, [getDataFromThingSpeak]);

  const onSubmit = async (data: ConfigFormValues) => {
    sendDataToThingSpeak(data.muted ? 1 : 0, data.active ? 1 : 0);
  };

  return (
    <div className="container mx-auto py-20 grid gap-4 grid-cols-1">
      <Card>
        <div className="flex items-center justify-between h-full px-5 py-5">
          <h1 className="text-sm font-semibold">Network Status: {networkStatus ? "Connected" : "Disconnected"}</h1>
        </div>
      </Card>
      <Form {...form}>
        <form onSubmit={form.handleSubmit(onSubmit)} className="grid grid-cols-1 gap-4">
          <Card>
            <div className="flex items-center justify-between h-full px-5 py-5">
              <h1 className="text-sm font-semibold">Muted</h1>
              <FormField
                control={form.control}
                name="muted"
                render={({ field }) => (
                  <FormItem>
                    <FormControl>
                      <Switch
                        checked={field.value}
                        onCheckedChange={field.onChange}
                      />
                    </FormControl>
                    <FormMessage />
                  </FormItem>
                )}
              />
            </div>
          </Card>
          <Card>
            <div className="flex items-center justify-between h-full px-5 py-5">
              <h1 className="text-sm font-semibold">Active</h1>
              <FormField
                control={form.control}
                name="active"
                render={({ field }) => (
                  <FormItem>
                    <FormControl>
                      <Switch
                        checked={field.value}
                        onCheckedChange={field.onChange}
                      />
                    </FormControl>
                    <FormMessage />
                  </FormItem>
                )}
              />
            </div>
          </Card>

          <Button type="submit" className="w-full">
            Save Changes
          </Button>
        </form>

      </Form>
    </div>
  );
}

